/*
 *  RasterMapManager.cpp
 *  GlassCockpit
 *
 *  Created by Hugo Vincent on 22/02/09.
 *  Copyright 2009 Hugo Vincent. All rights reserved.
 *
 */

#include "RasterMapManager.h"
#include "Debug.h"
#include "tinyjpeg.h"

#define MGM_CACHE_TILES_PER_FILE 32

namespace OpenGC {

RasterMapManager::RasterMapManager()
{

}

void RasterMapManager::SetCachePath(RasterMapManager::CacheFormat format, const std::string&)
{

}

RasterMapTile *RasterMapManager::GetTile(const unsigned int zoom, const unsigned int x, const unsigned int y)
{	
	if (zoom == 0 && x == 0 && y == 0)
	{
		FILE *mgmFile = fopen("/Users/hugo/Projects/iPhone/GlassCockpit/Data/MGMapsCache/GoogleTer_0/0_0.mgm", "rb");
		unsigned int width = 0, height = 0;
		unsigned char *img = ReadJPEG(mgmFile, 2 + 6 * MGM_CACHE_TILES_PER_FILE, 13786-194, width, height);
		return new RasterMapTile(img, width, height);
		fclose(mgmFile);
	}
	return NULL;
}

unsigned char *RasterMapManager::ReadJPEG(FILE *fp, unsigned int offset, unsigned int length, unsigned int& width, unsigned int& height)
{
	unsigned char *jpegBuffer = new unsigned char[length];
	fseek(fp, offset, SEEK_SET);
	fread(jpegBuffer, 1, length, fp);

	struct jdec_private *jdec = tinyjpeg_init();
	Assert(tinyjpeg_parse_header(jdec, jpegBuffer, length) >= 0, tinyjpeg_get_errorstring(jdec));
	
	tinyjpeg_decode(jdec, TINYJPEG_FMT_RGB24);
	unsigned char *components[3]; // this is how tinyjpeg does things...
	tinyjpeg_get_components(jdec, components);
	tinyjpeg_get_size(jdec, &width, &height);
//	tinyjpeg_free(jdec); // FIXME
	delete[] jpegBuffer;
	
	return components[0]; // FIXME re freeing above...
}

} // end namespace OpenGC
