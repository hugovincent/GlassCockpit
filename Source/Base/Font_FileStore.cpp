/*
 *  Font_FileStore.cpp
 *  GlassCockpit
 *
 *  Created by Hugo Vincent on 9/02/09.
 *  Copyright 2009 Hugo Vincent. All rights reserved.
 *
 */

#include "Debug.h"
#include <math.h>
#include "Font_FileStore.h"

#define FIRST_CHAR ' '
#define LAST_CHAR  '~'
#define NUM_CHARS (unsigned int)(LAST_CHAR - FIRST_CHAR + 1)

#define max(x,y) (x > y ? x : y)

#define MAX_TEXTURE_DIMENSION	1024
#define FONT_TEXTURE_SIZE		24.0 // size to render at - 50.0 gives a 512x512px texture, 24.0 gives a 256x256px texture

#ifdef USE_FREETYPE2 // i.e. on host platform, have libfreetype2 available

void computePowerOfTwo(GLshort inDim, GLshort *outDim)
{	
	if (!((inDim == 1) || !(inDim & (inDim - 1))))
	{
		unsigned int i = 1;
		while(i < inDim) i <<= 1;
		*outDim = i;
	}
	else 
		*outDim = inDim;
}

#include <ft2build.h>
#include <freetype/freetype.h>
Font_FileStore *Font_FileStore::CreateFromTTF(const std::string& ttfFilename)
{
	int glyphWidth = 0, glyphHeight = 0, rows, cols;
	FT_UInt *charIdx = 0;
	FT_GlyphSlot glyph;
	
	Font_FileStore *self = new Font_FileStore();
	self->store = new DiskFormat;
	memset(self->store, 0, sizeof(DiskFormat));
	self->store->firstGlyph = FIRST_CHAR;
	self->store->numGlyphs = NUM_CHARS;
	self->store->faceSize = FONT_TEXTURE_SIZE;

	self->store->glyphs = new Glyph[self->store->numGlyphs];
	memset(self->store->glyphs, 0, sizeof(Glyph[self->store->numGlyphs]));
	
	// Get font
	LogPrintf("Attempting to open %s... ", ttfFilename.c_str());
	FT_Library ftLibrary;
	FT_Init_FreeType(&ftLibrary);
	FT_Face ftFace;
	FT_Error err = FT_New_Face(ftLibrary, ttfFilename.c_str(), 0, &ftFace);
	if(err)
	{
		LogPrintf("Failed!\n");
		goto failed;
	}
	else
		LogPrintf("Opened.\n");
	
	// Get Freetype character-index mapping
	charIdx = new FT_UInt[self->store->numGlyphs];
	for (char i = 0; i < self->store->numGlyphs; ++i)
	{
		charIdx[i] = FT_Get_Char_Index(ftFace, FIRST_CHAR + i);
		if (charIdx[i] == 0) // undefined character code
		{
			LogPrintf("Failed to get font glyph index for character '%c'\n", i + FIRST_CHAR);
			goto failed;
		}
	}
	
	// Set font options
	FT_Face_SetUnpatentedHinting(ftFace, false);
	
	// Set font size
	err = FT_Set_Char_Size(ftFace, 0L, FONT_TEXTURE_SIZE * 64, 72, 72);
	if (err)
	{
		LogPrintf("Failed to set font size\n");
		goto failed;
	}
	
	// Compute kerning table
	if (FT_HAS_KERNING(ftFace) != 0)
	{
		self->store->kerningTable = new float[self->store->numGlyphs * self->store->numGlyphs];
		memset(self->store->kerningTable, 0, sizeof(float[self->store->numGlyphs * self->store->numGlyphs]));
		FT_Vector kernAdvance;
		
		for(unsigned int j = 0; j < self->store->numGlyphs; j++)
		{
			for(unsigned int i = 0; i < self->store->numGlyphs; i++)
			{
				err = FT_Get_Kerning(ftFace, charIdx[i], charIdx[j], FT_KERNING_UNFITTED, &kernAdvance);
				if(err)
					goto failed;

				self->store->kerningTable[j * self->store->numGlyphs + i] = static_cast<float>(kernAdvance.x) / 64.0f;
				// ^^^ the divide by 64.0 converts FreeType's 26.6-format fixed point into floating point
			}
		}
		LogPrintf("Computed font kerning table.\n");
	}
	else {
		self->store->kerningTable = NULL;
	}

	// Prerender characters to find maximum pixel size of a glyph, and while we're here, store the advances
	glyph = ftFace->glyph;
	for (char i = 0; i < self->store->numGlyphs; ++i)
	{
		// FIXME it's kindof wasteful prerendering the whole character set just to get the maximum
		// glyph size, but hey, font rendering is (necessarily) very cheap these days, and this is not called at runtime.
		err = FT_Load_Glyph(ftFace, charIdx[i], FT_LOAD_NO_HINTING | FT_LOAD_RENDER | FT_LOAD_NO_BITMAP);
		if(err || glyph->format != FT_GLYPH_FORMAT_BITMAP)
		{
			LogPrintf("Error 0x%02x prerendering glyph \"%c\" (%d)\n", err, i + FIRST_CHAR, charIdx[i]);
			goto failed;
		}
		
		// Get glyph size
		glyphWidth = max(glyphWidth, glyph->bitmap.width);
		glyphHeight = max(glyphHeight, glyph->bitmap.rows);
		
		// Store advance
		self->store->glyphs[i].advance = glyph->linearHoriAdvance / 65535.f; // fixed point -> float
	}
	LogPrintf("Computed max glyph size as %dx%d.\n", glyphWidth, glyphHeight);
	
	// Work out number of rows and columns
	rows = (int)ceilf(sqrtf((float)self->store->numGlyphs)), cols = (int)ceilf((float)self->store->numGlyphs / rows);
	LogPrintf("Texture has %d rows x %d cols (%d cells for %d glyphs).\n", rows, cols, rows*cols, self->store->numGlyphs);
	
	// Create texture
	self->store->rows = rows;
	self->store->columns = cols;
	self->store->glyphWidth = glyphWidth;
	self->store->glyphHeight = glyphHeight;

	GLshort tmp;
	computePowerOfTwo(self->store->glyphWidth * rows, &tmp);
	self->store->texWidth = tmp;
	computePowerOfTwo(self->store->glyphHeight * cols, &tmp);
	self->store->texHeight = tmp;
	
	self->store->bitmap = new GLubyte[self->store->texWidth * self->store->texHeight];
	memset(self->store->bitmap, 0, sizeof(GLubyte[self->store->texWidth * self->store->texHeight]));
	LogPrintf("Texture has size %dx%d px.\n", self->store->texWidth, self->store->texHeight);
	
	Assert(self->store->texWidth < MAX_TEXTURE_DIMENSION && self->store->texHeight < MAX_TEXTURE_DIMENSION, "texture is too big");
	
	// Now render each glyph
	for (char i = 0; i < self->store->numGlyphs; ++i)
	{
		// Load glyph into font face
		err = FT_Load_Glyph(ftFace, charIdx[i], FT_LOAD_DEFAULT);
		if(err)
		{
			LogPrintf("Error 0x%02x loading glyph \"%c\" (%d)\n", err, i + FIRST_CHAR, charIdx[i]);
			goto failed;
		}
		
		// Convert the glyph to a bitmap
		FT_Render_Glyph(ftFace->glyph, FT_RENDER_MODE_NORMAL);
		Assert(ftFace->glyph->format == FT_GLYPH_FORMAT_BITMAP, "rendered to a bitmap, but now it's not a bitmap");
		Assert(ftFace->glyph->bitmap.pitch >= 0, "negative pitch (currently?) unsupported");
		
		// Get the offset of this glyph into the shared texture image
		int tx, ty;
		self->TextureCellForCharacter(i, &tx, &ty);
		
		// Copy rendered glyph into texture
		GLubyte *tex = self->store->bitmap; // alias for brievity
		FT_Bitmap& bm = ftFace->glyph->bitmap; // alias for brievity
		for (int j = 0; j < glyphHeight; ++j)
			for(int k = 0; k < glyphWidth; ++k)
				tex[((ty * glyphHeight) + j) * self->store->texWidth + (tx * glyphWidth) + k] = 
					(k >= bm.width || j >= bm.rows) ? 0 : bm.buffer[(j * bm.pitch) + k];
		
		// Store glyph sizes and pixel offset (used to sit all characters on a baseline when we render)
		self->store->glyphs[i].width = bm.width;
		self->store->glyphs[i].height = bm.rows;
		self->store->glyphs[i].xOffset = ftFace->glyph->bitmap_left;
		self->store->glyphs[i].yOffset = ftFace->glyph->bitmap_top - bm.rows;
	}
	
	LogPrintf("Sucessfully created font store.\n");

	// Cleanup Freetype
	FT_Done_Face(ftFace);
	FT_Done_FreeType(ftLibrary);
	delete[] charIdx;
	
	return self;

failed:
	LogPrintf("Failed to create font store.\n");
	FT_Done_Face(ftFace);
	FT_Done_FreeType(ftLibrary);
	delete self;
	if (charIdx) delete[] charIdx;
	return NULL;
}
#endif

Font_FileStore::DiskFormat::~DiskFormat()
{
	if (glyphs)
		delete[] glyphs;
	if (kerningTable) 
		delete[] kerningTable;
	if (bitmap)
		delete[] bitmap;
}

Font_FileStore::Font_FileStore(const std::string& glfontFilename)
	: store(NULL)
{
	DeserializeFromFile(glfontFilename);
}

Font_FileStore::~Font_FileStore()
{
	if (store)
	{
		delete store;
		store = NULL;
	}
}

#define GLYPH(x) (store->glyphs[(x - store->firstGlyph)])
#define LAST_GLYPH (store->firstGlyph + store->numGlyphs)

GLfloat Font_FileStore::Advance(char i, char j)
{
	if (j == 0)
	{
		return GLYPH(i).advance;
	}
	else if ((i >= store->firstGlyph) && (i <= LAST_GLYPH) && (j >= store->firstGlyph) && (j <= LAST_GLYPH))
	{
		if (store->kerningTable)
			return GLYPH(i).advance + store->kerningTable[(j - store->firstGlyph)
											* store->numGlyphs + (i - store->firstGlyph)];
		else
			return GLYPH(i).advance;
	}
	else
	{
		return 0.f;
	}
	// FIXME scaling?
}

GLfloat *Font_FileStore::TextureCoordsForChar(char glyph)
{
	int tx, ty;
	TextureCellForCharacter(glyph - store->firstGlyph, &tx, &ty);
	
	GLfloat a = (GLfloat)store->glyphWidth / store->texWidth;
	GLfloat b = (GLfloat)store->glyphHeight / store->texHeight;
	GLfloat c = (GLfloat)GLYPH(glyph).width / store->glyphWidth;
	GLfloat d = (GLfloat)GLYPH(glyph).height / store->glyphHeight;
	
	texCoords[0] = a * tx;         texCoords[1] = b * (ty + d);
	texCoords[2] = a * (tx + c);   texCoords[3] = b * (ty + d);
	texCoords[4] = a * tx;         texCoords[5] = b * ty;
	texCoords[6] = a * (tx + c);   texCoords[7] = b * ty;

	return &texCoords[0];
}

GLfloat *Font_FileStore::VertexCoordsForChar(char glyph)
{
	// Baseline offset correction
	GLfloat xOff = GLYPH(glyph).xOffset; // FIXME
	GLfloat yOff = GLYPH(glyph).yOffset; // FIXME

	// Construct the vertices from m_FaceSize and glyph metrics
	GLfloat a = (GLfloat)GLYPH(glyph).width / store->texWidth * store->faceSize * 10.5f; // FIXME
	GLfloat b = (GLfloat)GLYPH(glyph).height / store->texHeight * store->faceSize * 10.5f; // FIXME
	
	vertexCoords[0] = xOff;	    vertexCoords[1] = yOff;
	vertexCoords[2] = xOff + a;	vertexCoords[3] = yOff;
	vertexCoords[4] = xOff;	    vertexCoords[5] = yOff + b;
	vertexCoords[6] = xOff + a;	vertexCoords[7] = yOff + b;
	
	return &vertexCoords[0];
}

GLubyte *Font_FileStore::TextureBitmap(unsigned int *texWidth, unsigned int *texHeight, bool *safeToFree)
{
	if (store && store->bitmap)
	{
		*safeToFree = true;
		*texWidth = store->texWidth;
		*texHeight = store->texHeight;
		GLubyte *ret = store->bitmap;
		store->bitmap = NULL;
		return ret;
	}
	else
		return NULL;
}
