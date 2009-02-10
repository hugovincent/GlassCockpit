/*
 *  Font_FileStore.h
 *  GlassCockpit
 *
 *  Created by Hugo Vincent on 9/02/09.
 *  Copyright 2009 Hugo Vincent. All rights reserved.
 *
 */

#ifndef Font_FileStore_H
#define Font_FileStore_H

#define USE_FREETYPE2 // FIXME undef for iPhone

#include <string>
#include <OpenGL/gl.h>

class Font_FileStore
{
public:
#ifdef USE_FREETYPE2
	static Font_FileStore *CreateFromTTF(const std::string& ttfFilename);
#endif
	
	Font_FileStore(const std::string& glfontFilename); // Convenience constructor for deserialize from file
	~Font_FileStore();

	void DeserializeFromFile(const std::string& glfontFilename);
	void SerializeToFile(const std::string& glfontFilename);
	
	// Methods for accessing the data
	
	GLfloat Advance(char i, char j); // for character i (followed by j, needed for kerning); in texture UV coordinates
	GLfloat *TextureCoordsForFloat(char glyph);
	GLbyte *TextureBitmap(unsigned int *texWidth, unsigned int *texHeight);
	
private:
	
	Font_FileStore() : store(NULL) {} // used to create from TTF for serialization
	
	class Glyph
	{
	public:
		char character;
		float advance;
		// FIXME I think we need baseline offset in here too?
	};
	
	class DiskFormat
	{
	public:
		// Glyph info
		char firstGlyph;
		int numGlyphs;
		Glyph *glyphs; // length =  numGlyphs
		float *kerningTable; // kerning advance for character pair i,j indexed by (j * numGlyphs + i)
		
		// Texture format info
		int rows, columns;
		int glyphWidth, glyphHeight; // maximum size of a glyph, actual size may be smaller
		int texWidth, texHeight;
		GLbyte *bitmap; // length = texWidth * texHeight
		
		// Note: pixel format is 8-bit alpha-only
	
		~DiskFormat();
	};
	
	DiskFormat *store;
	GLfloat texCoords[8]; // four (x,y) coordinates
};

#endif
