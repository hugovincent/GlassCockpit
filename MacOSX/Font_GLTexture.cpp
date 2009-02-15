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

Font_GLTexture::Font_GLTexture(const char* filename)
{
	LogPrintf("Opening font %s\n", filename);
	//m_Store = new Font_FileStore(filename);
	m_Store = Font_FileStore::CreateFromTTF(filename); // fixme should deserialize, obv
	
	// FIXME get bitmap and create textures in GL
	/* The format is GL_ALPHA and the params are
	 * GL_TEXTURE_WRAP_S = GL_CLAMP
	 * GL_TEXTURE_WRAP_T = GL_CLAMP
	 * GL_TEXTURE_MAG_FILTER = GL_LINEAR
	 * GL_TEXTURE_MIN_FILTER = GL_LINEAR
	 * Note that mipmapping is NOT used */
}
	
Font_GLTexture::~Font_GLTexture()
{
	delete m_Store;
}

void Font_GLTexture::Render(const char* str)
{
	int len = strlen(str);
	for (int i = 0; i < len; ++i)
	{
		glBegin(GL_QUADS);
		// FIXME construct the vertices from m_FaceSize and the advance of the glyph
		/*
		 // Set vertices
		 GLfloat c = 0.5f * contentWidth, d = 0.5f * contentHeight;
		 vertices[0] = -c; vertices[1] = -d;
		 vertices[2] =  c; vertices[3] = -d;
		 vertices[4] = -c; vertices[5] =  d;
		 vertices[6] =  c; vertices[7] =  d;
		 */
		glEnd();
		glTexCoordPointer(2, GL_FLOAT, 0, m_Store->TextureCoordsForFloat('a'));
		// FIXME render fragment
	}
}
		
float Font_GLTexture::Advance(const char* str)
{
	int len = strlen(str);
	float advance = 0.0;
	
	/*
	 FTUnicodeStringItr<T> ustr(string);
	 
	 for(int i = 0; (len < 0 && *ustr) || (len >= 0 && i < len); i++)
	 {
	 unsigned int thisChar = *ustr++;
	 unsigned int nextChar = *ustr;
	 
	 if(CheckGlyph(thisChar))
	 {
	 advance += glyphList->Advance(thisChar, nextChar);
	 }
	 }
	 */
	
	for (int i = 0; i < len; ++i)
	{
		advance += m_Store->Advance(str[i], str[i+1]); // FIXME
	}
	
	return advance * m_FaceSize;
}

int Font_GLTexture::Error()
{
	// Unsupported.
	return 0;
}
