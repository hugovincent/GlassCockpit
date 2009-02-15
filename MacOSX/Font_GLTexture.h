/*
 *  Font_GLTexture.h
 *  GlassCockpit
 *
 *  Created by Hugo Vincent on 8/02/09.
 *  Copyright 2009 Hugo Vincent. All rights reserved.
 *
 */

#ifndef Font_GLTexture_H
#define Font_GLTexture_H

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
	void FaceSize(float size) { m_FaceSize = size; }
	void Render(const char* str);
	float Advance(const char* str);
	int Error();

private:
	float m_FaceSize;
	Font_FileStore *m_Store;
};

#endif