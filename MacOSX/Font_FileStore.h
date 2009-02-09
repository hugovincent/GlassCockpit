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

#define REPR_HIGHER_RES 0
#define REPR_LOWER_RES 1

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
	
	GLfloat AdvanceForGlyph(int repr, char glyph); // in texture UV coordinates
	GLfloat *TextureCoordsForFloat(int repr, char glyph);
	GLbyte *TextureBitmap(int repr, unsigned int *texWidth, unsigned int *texHeight);
	
private:
	
	Font_FileStore() : store(NULL) {} // used to create from TTF for serialization
	
	class Glyph
	{
	public:
		char character;
		GLfloat advance; // x coordinate advance of the glyph in texture UV coordinates
	};
	
	class TextureRepr
	{
	public:
		int rows, columns;
		int glyphWidth, glyphHeight; // maximum size of a glyph, actual size may be smaller
		int texWidth, texHeight;
		GLbyte *bitmap; // length = texWidth * texHeight
		
		// Note: pixel format is 8-bit alpha-only
		
		~TextureRepr() { delete bitmap; }
	};
	
	class DiskFormat
	{
	public:
		// Glyph info
		char firstGlyph;
		int numGlyphs;
		Glyph *glyphs; // length =  numGlyphs
		
		// Texture format info
		int numReprs; // number of different-resolution texture representations. Always 2 for now
		TextureRepr *reprs[2];
		
		~DiskFormat() { delete glyphs; delete reprs; }
	};
	
	DiskFormat *store;
	GLfloat texCoords[8]; // four (x,y) coordinates
};

#endif
