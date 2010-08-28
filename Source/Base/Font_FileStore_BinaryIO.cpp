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

#include "Debug.h"
#include <math.h>
#include "Font_FileStore.h"

#include <fstream>

#if defined(TARGET_RT_BIG_ENDIAN) && TARGET_RT_BIG_ENDIAN

#if defined(__ppc__) && defined(__GNUC__)
	inline float EndianFixupFloat32(float x) {
		union { float f; uint32_t i; } tmp; tmp.f = x; uint32_t result;
		__asm__("lwbrx %0,0,%1" : "=r" (result) : "r" (&(tmp.i)), "m" (tmp.i));
		tmp.i = result; return tmp.f;
	}
#else
	inline float EndianFixupFloat32(float x) {
		union { float f; int i; } tmp; tmp.f = x;
		tmp.i = ((tmp.i & 0xFF) << 24) | ((tmp.i & 0xFF00) << 8) | ((tmp.i >> 8) & 0xFF00) | ((tmp.i >> 24) & 0xFF);
		return tmp.f;
	}
#endif

#if defined(__ppc__) && defined(__GNUC__)
	inline short EndianFixup16(short x) { uint16_t result; __asm__("lhbrx %0,0,%1" : "=r" (result) : "r" (&x), "m" (x)); return result; }
#else
	inline short EndianFixup16(short x) { return ((x << 8) & 0xFF00) | ((x >> 8) & 0xFF); }
#endif

#else
#if !defined(TARGET_RT_LITTLE_ENDIAN) || !TARGET_RT_LITTLE_ENDIAN
#warning No target endianess defined, assuming little-endian
#endif
	inline short EndianFixup16(short x) { return x; }
	inline float EndianFixupFloat32(float x) { return x; }
	// No action needed for little-endian targets
#endif


void Font_FileStore::CheckBinaryFormatAssumptions()
{
	// Because we serialize the font data, we need to be sure that our primitive type sizes are as expected.
	Assert(sizeof(char) == 1 && sizeof(short) == 2 && sizeof(int) == 4 && sizeof(float) ==4 && \
		   sizeof(GLubyte) == 1, "primitive data type size difference");
	
	// Check floating point is in the correct format
	float tmp = 1234.5678, tmp_orig = tmp;
	EndianFixupFloat32(tmp);
	EndianFixupFloat32(tmp);
	Assert(tmp == tmp_orig, "floating point format difference");
}

static const char DiskHeader[4] = {'F', 'O', 'N', 'T'};

void Font_FileStore::SerializeToFile(const std::string& glfontFilename) const
{
	// Note: it is assumed the serialization will always occur on a little-endian machine

	if (store)
	{
		CheckBinaryFormatAssumptions();

		LogPrintf("Serializing font to %s... ", glfontFilename.c_str());
		
		std::ofstream out(glfontFilename.c_str(), std::ios::binary | std::ios::trunc);
		out.write(DiskHeader, sizeof(DiskHeader));
		out.write((char*)&store->faceSize, sizeof(float));
		out.write((char*)&store->firstGlyph, sizeof(char));
		out.write((char*)&store->numGlyphs, sizeof(short));
		out.write((char*)&store->rows, sizeof(short));
		out.write((char*)&store->columns, sizeof(short));
		out.write((char*)&store->glyphWidth, sizeof(short));
		out.write((char*)&store->glyphHeight, sizeof(short));
		out.write((char*)&store->texWidth, sizeof(short));
		out.write((char*)&store->texHeight, sizeof(short));
		
		for (int i = 0; i < store->numGlyphs; ++i)
		{
			out.write((char*)&(store->glyphs[i].character), sizeof(char));
			out.write((char*)&(store->glyphs[i].xOffset), sizeof(short));
			out.write((char*)&(store->glyphs[i].yOffset), sizeof(short));
			out.write((char*)&(store->glyphs[i].width), sizeof(short));
			out.write((char*)&(store->glyphs[i].height), sizeof(short));
			out.write((char*)&(store->glyphs[i].advance), sizeof(float));
		}
		
		out.write((char*)store->kerningTable, sizeof(float) * store->numGlyphs * store->numGlyphs);
		out.write((char*)store->bitmap, sizeof(GLubyte) * store->texWidth * store->texHeight);
		out.close();
		LogPrintf("Done.\n");
	}
}

void Font_FileStore::DeserializeFromFile(const std::string& glfontFilename)
{
	if (store) delete store;
	CheckBinaryFormatAssumptions();
		
	LogPrintf("Deserializing font %s... ", glfontFilename.c_str());
	
	// Variables used to hold read-in values
	short tmps; float tmpf; char tmpc;
	
	std::ifstream in(glfontFilename.c_str(), std::ios::binary);
	char read_header[sizeof(DiskHeader)];
	in.read(read_header, sizeof(DiskHeader));
	Assert(memcmp(read_header, DiskHeader, sizeof(DiskHeader)) == 0, "file header does not match");
	
	DiskFormat *format = new DiskFormat();
	in.read((char*)&tmpf, sizeof(float));
	format->faceSize = EndianFixupFloat32(tmpf);
	in.read((char*)&tmpc, sizeof(char));
	format->firstGlyph = tmpc;
	in.read((char*)&tmps, sizeof(short));
	format->numGlyphs = EndianFixup16(tmps);
	in.read((char*)&tmps, sizeof(short));
	format->rows = EndianFixup16(tmps);
	in.read((char*)&tmps, sizeof(short));
	format->columns = EndianFixup16(tmps);
	in.read((char*)&tmps, sizeof(short));
	format->glyphWidth = EndianFixup16(tmps);
	in.read((char*)&tmps, sizeof(short));
	format->glyphHeight = EndianFixup16(tmps);
	in.read((char*)&tmps, sizeof(short));
	format->texWidth = EndianFixup16(tmps);
	in.read((char*)&tmps, sizeof(short));
	format->texHeight = EndianFixup16(tmps);
	
	// Sanity check the read-in data to make sure we didn't get some kind of endianess or struct packing problem
	Assert(format->faceSize > 0 && format->faceSize < 500 && format->rows > 1
		   && format->rows < 100 && format->columns > 1 && format->columns < 100, "data format error suspected");
	
	format->glyphs = new Glyph[format->numGlyphs];
	for (int i = 0; i < format->numGlyphs; ++i)
	{
		in.read((char*)&tmpc, sizeof(char));
		format->glyphs[i].character = tmpc;
		in.read((char*)&tmps, sizeof(short));
		format->glyphs[i].xOffset = EndianFixup16(tmps);
		in.read((char*)&tmps, sizeof(short));
		format->glyphs[i].yOffset = EndianFixup16(tmps);
		in.read((char*)&tmps, sizeof(short));
		format->glyphs[i].width = EndianFixup16(tmps);
		in.read((char*)&tmps, sizeof(short));
		format->glyphs[i].height = EndianFixup16(tmps);
		in.read((char*)&tmpf, sizeof(float));
		format->glyphs[i].advance = EndianFixupFloat32(tmpf);
	}
		 
	format->kerningTable = new float[format->numGlyphs * format->numGlyphs];
	in.read((char*)format->kerningTable, sizeof(float) * format->numGlyphs * format->numGlyphs);
	for (int i = 0; i < (format->numGlyphs * format->numGlyphs); ++i)
		format->kerningTable[i] = EndianFixupFloat32(format->kerningTable[i]);
	
	format->bitmap = new GLubyte[format->texWidth * format->texHeight];
	in.read((char*)format->bitmap, sizeof(GLubyte) * format->texWidth * format->texHeight);
	store = format;
	LogPrintf("Done.\n");
	
	// Check there is no more data in the file (indicating a different binary format than expected)
	int count = 0; char tmp;
	while (!in.eof())
		in.read(&tmp, 1) && count++;

	if (count >= 1)
	{
		printf("Error... %d bytes more data in file\n", count);
		Assert(count <= 1, "excess data at end of serialized font file");
	}
}
