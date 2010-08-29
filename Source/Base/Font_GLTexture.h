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

#ifndef Font_GLTexture_H
#define Font_GLTexture_H

#include "GLHeaders.h"

class Font_FileStore;

/** This is a minimal, lightweight reimplementation of FTTextureFont from
 *  the FTGL library, for use on OpenGL ES, e.g. on the iPhone. It only
 *  provides the functionality required by OpenGC. 
 *
 *  We acheive this reduction in CPU and memory required by prerendering
 *  the font to a bitmap image on the development host, where we have access
 *  to the full stack, in particular libfreetype2.
 *
 *  Only 7-bit ASCII is supported, no unicode.
 */
class Font_GLTexture
{
public:
	Font_GLTexture(const char* filename);
	~Font_GLTexture();
	void FaceSize(float size);
	void Render(const char* str);
	float Advance(const char* str);
	int Error();

private:
	Font_FileStore *m_Store;
	GLuint texName;
	
	void CheckOrCreateTexture();
};

#endif
