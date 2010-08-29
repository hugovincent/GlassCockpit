/*=========================================================================

  Copyright (c) 2005-2010 Hugo Vincent <hugo.vincent@gmail.com>
  All rights reserved.
  
  This project is distributed under the terms of the GNU General Public License
  Version 3 <http://www.gnu.org/licenses/gpl.html>.
  
      This program is free software: you can redistribute it and/or modify
      it under the terms of the GNU General Public License as published by
      the Free Software Foundation, specifically version 3 of the License.
  
      This program is distributed in the hope that it will be useful,
      but WITHOUT ANY WARRANTY; without even the implied warranty of
      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
      GNU General Public License for more details.
  
      You should have received a copy of the GNU General Public License
      along with this program.  If not, see <http://www.gnu.org/licenses/>.

=========================================================================*/

#include <stdio.h>
#include <string.h>
#include "Debug.h"
#include "Font_GLTexture.h"
#include "Font_FileStore.h"

Font_GLTexture::Font_GLTexture(const char* filename)
{
	m_Store = new Font_FileStore(filename);

	// For debugging, create the font FileStore fresh
	//m_Store = Font_FileStore::CreateFromTTF(filename);
	
	texName = 0; // defer creating GL texture until we're sure we have a valid GL context
}
	
Font_GLTexture::~Font_GLTexture()
{
	delete m_Store;
	if (texName)
		glDeleteTextures(1, &texName);
}

void Font_GLTexture::CheckOrCreateTexture()
{
	if (texName)
		return;
	
	// Get the bitmap and create textures in GL
	glGenTextures(1, &texName);
	glBindTexture(GL_TEXTURE_2D, texName);
	
	unsigned int texWidth, texHeight; bool safeToFree;
	GLubyte *bitmap = m_Store->TextureBitmap(&texWidth, &texHeight, &safeToFree);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, texWidth, texHeight, 0, GL_ALPHA, GL_UNSIGNED_BYTE, bitmap);
	if (safeToFree) delete[] bitmap;
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
}

void Font_GLTexture::Render(const char* str)
{
	CheckOrCreateTexture();

	// Store old state for restoring afterwards
	GLboolean oldVertex, oldTexCoords, oldBlending;
	GLint oldBlendSrc, oldBlendDst;
	glGetBooleanv(GL_VERTEX_ARRAY, &oldVertex);
	glGetBooleanv(GL_TEXTURE_COORD_ARRAY, &oldTexCoords);
	glGetBooleanv(GL_BLEND, &oldBlending);
	glGetIntegerv(GL_BLEND_SRC, &oldBlendSrc);
	glGetIntegerv(GL_BLEND_DST, &oldBlendDst);
	
	// Enable GL options
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture( GL_TEXTURE_2D, texName);
	int len = strlen(str);
	for (int i = 0; i < len; ++i)
	{
		glTexCoordPointer(2, GL_FLOAT, 0, m_Store->TextureCoordsForChar(str[i]));
		glVertexPointer(2, GL_FLOAT, 0, m_Store->VertexCoordsForChar(str[i]));
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glTranslatef(m_Store->Advance(str[i], (i < len) ? str[i+1] : 0), 0.f, 0.f);
	}
	glDisable(GL_TEXTURE_2D);
	
	// Restore old state
	if (oldTexCoords == GL_FALSE)
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	if (oldVertex == GL_FALSE)
		glDisableClientState(GL_VERTEX_ARRAY);
	if (oldBlending == GL_FALSE)
		glDisable(GL_BLEND);
	if (oldBlendSrc != GL_SRC_ALPHA || oldBlendDst != GL_ONE_MINUS_SRC_ALPHA)
		glBlendFunc(oldBlendSrc, oldBlendDst);
}

void Font_GLTexture::FaceSize(float size)
{
	Assert(m_Store && m_Store->FaceSize() == size, "prerendered font size disagreement");
}
		
float Font_GLTexture::Advance(const char* str)
{
	int len = strlen(str);
	float advance = 0.0;

	for (int i = 0; i < len; ++i)
	{
		advance += m_Store->Advance(str[i], (i < len) ? str[i + 1] : 0);
	}

	return advance;
}

int Font_GLTexture::Error()
{
	// Unsupported.
	return 0;
}
