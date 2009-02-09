/*
 *  Font_GLTexture.cpp
 *  GlassCockpit
 *
 *  Created by Hugo Vincent on 8/02/09.
 *  Copyright 2009 Hugo Vincent. All rights reserved.
 *
 */

#include <stdio.h>
#include "Debug.h"
#include "Font_GLTexture.h"
#include "Font_FileStore.h"

/*
 * The format is GL_ALPHA and the params are
 * GL_TEXTURE_WRAP_S = GL_CLAMP
 * GL_TEXTURE_WRAP_T = GL_CLAMP
 * GL_TEXTURE_MAG_FILTER = GL_LINEAR
 * GL_TEXTURE_MIN_FILTER = GL_LINEAR
 * Note that mipmapping is NOT used
 */

Font_GLTexture::Font_GLTexture(const char* filename)
{
	printf("opening font %s\n", filename);
	m_Store = new Font_FileStore(filename);
}
	
Font_GLTexture::~Font_GLTexture()
{
	delete m_Store;
}

void Font_GLTexture::Render(const char* str)
{
	// for each character in str, this has to construct the vertices from
	// m_FaceSize and the advance of the glyph, then get the texture coordinates 
	// and render the fragment.
	//GLfloat *texCoords = m_Store->TextureCoordsForFloat('a');
}
		
float Font_GLTexture::Advance(const char* str)
{
	int len = strlen(str);
	float advance = 0.0;
	
	for (int i = 0; i < len; ++i)
	{
		advance += m_Store->AdvanceForGlyph(str[i]);
	}
	
	return advance;
}

void Font_GLTexture::Hint_LowerResolution(bool lower)
{
	// if lower == true, that means we can get away with using half the texture resolution
}

int Font_GLTexture::Error()
{
	return 0;
}
