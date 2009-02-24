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
#include "math.h"

/*** Header and tile information ***/
#define MGM_CACHE_TILES_PER_FILE 32 // FIXME generalise to arbirary number of tiles per cache file using cache.conf
#define TILE_SIZE_PIXELS 256
typedef struct {
	unsigned char relX, relY;
	unsigned char nextOffset[4];
} MGMHeaderChunk;
#define CHUNK_NEXT_OFFSET(x) (x->nextOffset[0] << 24 | x->nextOffset[1] << 16 | x->nextOffset[2] << 8 | x->nextOffset[3])


namespace OpenGC {

RasterMapManager::RasterMapManager()
	: m_Ready(false)
{

}

void RasterMapManager::SetCachePath(CacheFormat format, const std::string& path, const std::string& mapType)
{
	if (format == RMM_CACHE_MGMAPS)
	{
		m_CachePrefix = path;
		m_MapType = mapType;
		m_Ready = true;
		LogPrintf("RasterMapManager: Loading MGMaps cache of type %s from %s\n", mapType.c_str(), path.c_str());
	}
}

#if 0
int lat2pixel(float zoom, float lat)
{
	return -((atanh(sin(lat)) * TILE_SIZE_PIXELS * exp(zoom * M_LN2) ) / (2*M_PI)) + 
		exp(zoom * M_LN2) * TILE_SIZE_PIXELS / 2;
}

int lon2pixel(float zoom, float lon)
{
	return lon * TILE_SIZE_PIXELS * exp(zoom * M_LN2) / 2 * M_PI + exp(zoom * M_LN2) * TILE_SIZE_PIXELS/2;
}

float pixel2lon(float zoom, int pixel_x)
{
	return (pixel_x - exp(zoom * M_LN2) * TILE_SIZE_PIXELS/2) * 2 * M_PI / (TILE_SIZE_PIXELS * exp(zoom * M_LN2));
}

float pixel2lat(float zoom, int pixel_y)
{
	return asin(tanh(pixel_y - exp(zoom * M_LN2) * TILE_SIZE_PIXELS / 2) * -2 * M_PI / (TILE_SIZE_PIXELS * exp(zoom * M_LN2)));
}
#endif

void GetTileCoordsForLatLon(unsigned int& x, unsigned int& y, double lat, double lon, const unsigned int zoom)
{
//	int mapsize = TILE_SIZE_PIXELS * 1 << zoom;
//	int origin = mapsize / 2;
	
	// Make lat/lon in 0->180, 0->360 instead of +/-90, +/-180
//	double latDef = fabs(-90.0 - lat);
//	double lonDeg = fabs(-180.0 - lon);
	
}

RasterMapTile *RasterMapManager::GetTile(const unsigned int zoom, const unsigned int x, const unsigned int y)
{	
	if (m_Ready)
	{
		// Work out what file our tile is in
		static char filenameBuf[1024];
		// FIXME munge x,y
		if (snprintf(filenameBuf, sizeof(filenameBuf), "%s/%s_%d/%d_%d.mgm", 
			m_CachePrefix.c_str(), m_MapType.c_str(), zoom, x, y) == sizeof(filenameBuf))
		{
			LogPrintf("RasterMapManager: Could not work out what file the requested tile is in.\n");
			return NULL;
		}
		// FIXME compute relX, relY
		signed char relX = 7, relY = 0;
		
		// Get the file
		FILE *mgmFile = fopen(filenameBuf, "rb");
		if (!mgmFile)
		{
			LogPrintf("RasterMapManager: Provided cache does not contain requested tile, ignoring\n");
			return NULL;
		}
		
		// Read MGMaps header
		static unsigned char mgmapsHeader[2 + 6 * MGM_CACHE_TILES_PER_FILE];
		if (fread(mgmapsHeader, 1, sizeof(mgmapsHeader), mgmFile) != sizeof(mgmapsHeader))
		{
			LogPrintf("RasterMapsManager: could not read MGMaps tile cache file header\n");
			return NULL;
		}

		// Parse MGMaps header, searching for desired tile
		unsigned int nextOffset = sizeof(mgmapsHeader), offset = 0;
		int numTiles = mgmapsHeader[0] << 8 | mgmapsHeader[1];
		Assert(numTiles >= 1 && numTiles <= MGM_CACHE_TILES_PER_FILE, "wrong number of tiles in cache file");
		for (int i = 0; i < numTiles; ++i)
		{
			MGMHeaderChunk *tmp = (MGMHeaderChunk*)(&mgmapsHeader[2 + i * sizeof(MGMHeaderChunk)]);
			offset = nextOffset;
			nextOffset = CHUNK_NEXT_OFFSET(tmp);
			if (tmp->relX == relX && tmp->relY == relY)
			{
				// Check that the computed length is sane
				fseek(mgmFile, 0, SEEK_END);
				if (ftell(mgmFile) < nextOffset)
				{
					LogPrintf("RasterMapsManager: error parsing MGMaps header (bad size)\n");
					return NULL;
				}
				
				// Get the tile from the cachefile
				unsigned int width = 0, height = 0;
				unsigned char *img = ReadJPEG(mgmFile, offset, nextOffset - offset, width, height);
				fclose(mgmFile);
				return new RasterMapTile(img, width, height);
			}
		}
		fclose(mgmFile);
	}
	LogPrintf("RasterMapsManager: could not find requested file in tile cache\n");
	return NULL;
}

unsigned char *RasterMapManager::ReadJPEG(FILE *fp, unsigned int offset, unsigned int length, unsigned int& width, unsigned int& height)
{
	if (length >= READ_BUFFER_SIZE)
	{
		LogPrintf("RasterMapManager: Attempt to read JPEG longer than read buffer, malformed cache.\n");
		return NULL;
	}
	
	fseek(fp, offset, SEEK_SET);
	fread(m_ReadBuffer, 1, length, fp);
	if (ftell(fp) < (offset + length))
	{
		LogPrintf("RasterMapManager: End of file reached before JPEG was completely read, malformed cache.\n");
		return NULL;
	}

	unsigned char *imgComponents[3];
	static struct jdec_private *jdec = 0;
	if (jdec != 0) 
	{
		tinyjpeg_free(jdec);
	}
	jdec = tinyjpeg_init();
	Assert(tinyjpeg_parse_header(jdec, m_ReadBuffer, length) >= 0, tinyjpeg_get_errorstring(jdec));
	
	tinyjpeg_decode(jdec, TINYJPEG_FMT_RGB24);
	tinyjpeg_get_components(jdec, imgComponents);
	tinyjpeg_get_size(jdec, &width, &height);
	
	return imgComponents[0];
}

} // end namespace OpenGC
