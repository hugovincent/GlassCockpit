/*
 *  Font_GLTexture.cpp
 *  GlassCockpit
 *
 *  Created by Hugo Vincent on 8/02/09.
 *  Copyright 2009 Hugo Vincent. All rights reserved.
 *
 */

#include <stdio.h>
#include "Font_GLTexture.h"

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
}
	
Font_GLTexture::~Font_GLTexture()
{

}

void Font_GLTexture::FaceSize(double size)
{
	
}

void Font_GLTexture::Render(const char* str)
{
	
}
		
float Font_GLTexture::Advance(const char* str)
{
	return 0.0;
}

void Font_GLTexture::Hint_LowerResolution(bool lower)
{

}

int Font_GLTexture::Error()
{
	return 0;
}
