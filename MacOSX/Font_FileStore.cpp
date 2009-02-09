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
#define NUM_CHARS (unsigned int)(LAST_CHAR - FIRST_CHAR)

#define max(x,y) (x > y ? x : y)

#define MAX_TEXTURE_DIMENSION 1024


#ifdef USE_FREETYPE2
#include <ft2build.h>
#include <freetype/freetype.h>
Font_FileStore *Font_FileStore::CreateFromTTF(const std::string& ttfFilename)
{
	Font_FileStore *self = new Font_FileStore();
	self->store = new DiskFormat;
	self->store->glyphs = new Glyph[NUM_CHARS];
	
	LogPrintf("Attempting to open %s... ", ttfFilename.c_str());
	FT_Library ftLibrary;
	FT_Init_FreeType(&ftLibrary);
	FT_Face *ftFace = new FT_Face();
	FT_Error err = FT_New_Face(ftLibrary, ttfFilename.c_str(), 0, ftFace);
	if(err)
	{
		LogPrintf("Failed!\n");
		delete ftFace;
		delete self;
		FT_Done_FreeType(ftLibrary);
		return NULL;
	}
	else
	{
		LogPrintf("Opened.\n");
	}
	
	// Preprocess characters to find metrics needed for texture sizing
	int maxGlyphWidth = 0, maxGlyphHeight = 0;
	for (char i = FIRST_CHAR; i <= LAST_CHAR; ++i)
	{
		// Get glyph size
		int glyphWidth = 4, glyphHeight = 4; // FIXME
		
		maxGlyphWidth = max(maxGlyphWidth, glyphWidth);
		maxGlyphHeight = max(maxGlyphHeight, glyphHeight);
	}
	
	// Work out number of rows and columns
	int rows = (int)ceilf(sqrtf((float)NUM_CHARS)), cols = (int)ceilf((float)NUM_CHARS / rows);
	LogPrintf("Texture has %d rows x %d cols ... %d cells for %d glyphs\n", rows, cols, rows*cols, NUM_CHARS);
	
	// Create textures
	self->store->numReprs = 2;
	
	self->store->reprs[0] = new TextureRepr();
	self->store->reprs[0]->rows = rows;
	self->store->reprs[0]->columns = cols;
	self->store->reprs[0]->glyphWidth = maxGlyphWidth;
	self->store->reprs[0]->glyphHeight = maxGlyphHeight;
	self->store->reprs[0]->texWidth = self->store->reprs[0]->glyphWidth * rows;
	self->store->reprs[0]->texHeight = self->store->reprs[0]->texHeight * cols;
	self->store->reprs[0]->bitmap = new GLbyte[self->store->reprs[0]->texWidth * self->store->reprs[0]->texHeight];
	
	Assert(self->store->reprs[0]->texWidth < MAX_TEXTURE_DIMENSION &&
		   self->store->reprs[0]->texHeight < MAX_TEXTURE_DIMENSION, "texture is too big");
	
	self->store->reprs[1] = new TextureRepr();
	self->store->reprs[1]->rows = rows;
	self->store->reprs[1]->columns = cols;
	self->store->reprs[1]->glyphWidth = maxGlyphWidth / 2;
	self->store->reprs[1]->glyphHeight = maxGlyphHeight / 2;
	self->store->reprs[1]->texWidth = self->store->reprs[1]->glyphWidth * rows;
	self->store->reprs[1]->texHeight = self->store->reprs[1]->glyphHeight * cols;
	self->store->reprs[1]->bitmap = new GLbyte[self->store->reprs[1]->texWidth * self->store->reprs[1]->texHeight];
	
	// Now process each character
	for (char i = FIRST_CHAR; i <= LAST_CHAR; ++i)
	{
		LogPrintf("Processing glyph for '%c'\n", i);
		
		// Render to repr[0]
		// FIXME
		
		// Render to repr[1]
		// FIXME
	}

	// Cleanup Freetype
	delete ftFace;
	FT_Done_FreeType(ftLibrary);
	
	return self;
}
#endif

Font_FileStore::Font_FileStore(const std::string& glfontFilename)
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

GLfloat Font_FileStore::AdvanceForGlyph(int repr, char glyph)
{
	if (store && (store->firstGlyph < glyph) && (glyph >= (store->firstGlyph + store->numGlyphs)))
	{
		return store->glyphs[glyph - store->firstGlyph].advance;
	}
	else
		return 0.f;
}

GLfloat *Font_FileStore::TextureCoordsForFloat(int repr, char glyph)
{
/*	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	GLuint curRow, curCol;
	if (storedOrientation == Orient_RowMajor) {
		
		curRow = (curFrame - 1) / rows;
		curCol = (curFrame - 1) % rows;
	} else {
		
		curRow = (curFrame - 1) % columns;
		curCol = (curFrame - 1) / columns;
	}
	glTranslatef(curRow * (GLfloat)contentWidth / width, curCol * (GLfloat)contentHeight / height, 0.f);
	glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
 */
	return &texCoords[0];
}

GLbyte *Font_FileStore::TextureBitmap(int repr, unsigned int *texWidth, unsigned int *texHeight)
{
	if (store)
	{
		*texWidth = store->reprs[repr]->texWidth;
		*texHeight = store->reprs[repr]->texHeight;
		return store->reprs[repr]->bitmap;
	}
	else
		return NULL;
}

void Font_FileStore::SerializeToFile(const std::string& glfontFilename)
{
	if (store)
	{
		LogPrintf("Serializing font to %s... ", glfontFilename.c_str());

		// FIXME

		LogPrintf("Done.\n");
	}
}

void Font_FileStore::DeserializeFromFile(const std::string& glfontFilename)
{
	
}
