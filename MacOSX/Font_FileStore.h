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
	
	GLfloat AdvanceForGlyph(char glyph);
	GLfloat *TextureCoordsForFloat(char glyph);
	GLbyte *TextureBitmap(unsigned int *texWidth, unsigned int *texHeight);
	
private:
	
	Font_FileStore() : store(NULL) {} // used to create from TTF for serialization
	
	class Glyph
	{
	public:
		char character;
		GLfloat advance; // x coordinate advance of the glyph
	};
	
	class TextureRepr
	{
	public:
		unsigned int rows, columns;
		unsigned int glyphWidth, glyphHeight; // maximum size of a glyph, actual size may be smaller
		unsigned int texWidth, texHeight;
		GLbyte *bitmap; // length = texWidth * texHeight (note pixel format is 8-bit alpha-only)
		
		~TextureRepr() { delete bitmap; }
	};
	
	class DiskFormat
	{
	public:
		// Glyph info
		char firstGlyph;
		unsigned int numGlyphs;
		Glyph *glyphs; // length =  numGlyphs
		
		// Texture format info
		unsigned int numReprs; // number of different-resolution texture representations. Always 2 for now
		TextureRepr *reprs[2];
		
		~DiskFormat() { delete glyphs; delete reprs; }
	};
	
	DiskFormat *store;
	GLfloat texCoords[8]; // four (x,y) coordinates
	unsigned int currentRepr;
};

#endif
