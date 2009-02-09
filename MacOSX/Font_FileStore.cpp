/*
 *  Font_FileStore.cpp
 *  GlassCockpit
 *
 *  Created by Hugo Vincent on 9/02/09.
 *  Copyright 2009 Hugo Vincent. All rights reserved.
 *
 */

#include "Debug.h"

#define FIRST_CHAR ' '
#define LAST_CHAR  '~'
#define NUM_CHARS (unsigned int)(LAST_CHAR - FIRST_CHAR)

#define USE_FREETYPE2
#include "Font_FileStore.h"

#ifdef USE_FREETYPE2
#include <ft2build.h>
Font_FileStore *Font_FileStore::CreateFromTTF(const std::string& ttfFilename)
{
	Font_FileStore *self = new Font_FileStore();
	LogPrintf("attempting to open %s\n", ttfFilename.c_str());
	self->store = new DiskFormat;
	self->store->glyphs = new Glyph[NUM_CHARS];
	
	// Preprocess characters to find metrics needed for texture sizing
	for (char i = FIRST_CHAR; i <= LAST_CHAR; ++i)
	{

	}
	
	// Create textures
	self->store->numReprs = 2;
	self->store->reprs[0] = new TextureRepr();
	self->store->reprs[0]->texWidth = 5;
	self->store->reprs[0]->texHeight = 5;
	self->store->reprs[0]->bitmap = new GLbyte[self->store->reprs[0]->texWidth * self->store->reprs[0]->texHeight];
	
	self->store->reprs[1] = new TextureRepr();
	self->store->reprs[1]->texWidth = self->store->reprs[0]->texWidth / 2;
	self->store->reprs[1]->texHeight = self->store->reprs[0]->texHeight / 2;
	self->store->reprs[1]->bitmap = new GLbyte[self->store->reprs[1]->texWidth * self->store->reprs[1]->texHeight];
	
	// Now process each character
	for (char i = FIRST_CHAR; i <= LAST_CHAR; ++i)
	{
		LogPrintf("Processing glyph for '%c'\n", i);
	}

	return self;
}
#endif

Font_FileStore::Font_FileStore(const std::string& glfontFilename)
{
	currentRepr = 0;
}

Font_FileStore::~Font_FileStore()
{
	if (store)
	{
		delete store;
		store = NULL;
	}
}

GLfloat Font_FileStore::AdvanceForGlyph(char glyph)
{
	if ((store->firstGlyph < glyph) && (glyph >= (store->firstGlyph + store->numGlyphs)))
	{
		return store->glyphs[glyph - store->firstGlyph].advance;
	}
	else
		return 0.f;
}

GLfloat *Font_FileStore::TextureCoordsForFloat(char glyph)
{
	// FIXME set texture coords for glyph
	return &texCoords[0];
}

GLbyte *Font_FileStore::TextureBitmap(unsigned int *texWidth, unsigned int *texHeight)
{
	if (store)
	{
		*texWidth = store->reprs[currentRepr]->texWidth;
		*texHeight = store->reprs[currentRepr]->texHeight;
		return store->reprs[currentRepr]->bitmap;
	}
	else
		return NULL;
}

void Font_FileStore::SerializeToFile(const std::string& glfontFilename)
{
	
}

void Font_FileStore::DeserializeFromFile(const std::string& glfontFilename)
{
	
}
