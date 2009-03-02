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
#include "Constants.h"
#include "tinyjpeg.h"
#include "math.h"
#include "lodepng.h"

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

/*
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
*/

#define ONE_MINUS 0.99999 // used to prevent numerical singularities at North/South poles
void RasterMapManager::GetTileCoordsForLatLon(unsigned int& x, unsigned int& y, float& fx, float& fy, 
											  double lat, double lon, const unsigned int zoom)
{
	int mapsize = TILE_SIZE_PIXELS * 1 << zoom;
	float origin = mapsize * 0.5;

	// Longitude -> Pixel coords
	float lonDeg = fabs(-180.0 - lon);
	fx = lonDeg * mapsize / 360.0;

	// Lattitude -> Pixel coords
	float e = sin(lat * DEG_TO_RAD);
	if (e > ONE_MINUS) e = ONE_MINUS;
	if (e < -ONE_MINUS) e = -ONE_MINUS;
	fy = origin + 0.5 * logf((1 + e) / (1 - e)) * (-mapsize / (2 * M_PI)); // FIXME not in desired interval

	// Tile coordinates
	x = fx / TILE_SIZE_PIXELS;
	y = fy / TILE_SIZE_PIXELS;

	// Pixel coordinates within that tile
	fx = fmodf(fx, TILE_SIZE_PIXELS);
	fy = fmodf(fy, TILE_SIZE_PIXELS);
}

RasterMapTile *RasterMapManager::GetTile(const unsigned int zoom, const unsigned int x, const unsigned int y)
{	
	if (m_Ready)
	{
		// Compute what file our tile is in (as implemented, this is dependent on MGM_CACHE_TILES_PER_FILE == 32)
		unsigned int absX = x >> 3, absY = y >> 2;
		unsigned char relX = x - absX * 8, relY = y - absY * 4;
		
		// Work out the file name
		static char filenameBuf[1024];
		if (snprintf(filenameBuf, sizeof(filenameBuf), "%s/%s_%d/%d_%d.mgm", 
			m_CachePrefix.c_str(), m_MapType.c_str(), zoom, absX, absY) == sizeof(filenameBuf))
		{
			LogPrintf("RasterMapManager: Could not work out what file the requested tile is in.\n");
			return NULL;
		}
		
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
				unsigned int length = nextOffset - offset;
				// Check that the computed length is sane
				fseek(mgmFile, 0, SEEK_END);
				if (ftell(mgmFile) < nextOffset)
				{
					LogPrintf("RasterMapsManager: error parsing MGMaps header (bad size)\n");
					return NULL;
				}

				// Check read buffer is big enough to hold tile
				if (length >= READ_BUFFER_SIZE)
				{
					LogPrintf("RasterMapManager: can't read JPEG longer than read buffer; probable malformed cache.\n");
					return NULL;
				}

				// Get the tile from the cachefile
				fseek(mgmFile, offset, SEEK_SET);
				if (length != fread(m_ReadBuffer, 1, length, mgmFile))
				{
					LogPrintf("RasterMapManager: end of file reached before JPEG was completely read, malformed cache.\n");
					return NULL;
				}
				fclose(mgmFile);
				
				// Decode the tile
				unsigned int width = 0, height = 0;
				unsigned char *img = DecodeJPEG(m_ReadBuffer, length, width, height);
				return new RasterMapTile(img, width, height);
			}
		}
		fclose(mgmFile);
	}
	LogPrintf("RasterMapsManager: could not find requested file in tile cache\n");
	return NULL;
}

unsigned char *RasterMapManager::DecodeJPEG(unsigned char *readBuffer, unsigned int length, unsigned int& width, unsigned int& height)
{
	unsigned char *imgComponents[3];
	static struct jdec_private *jdec = 0;
	if (jdec != 0) 
	{
		tinyjpeg_free(jdec);
	}
	jdec = tinyjpeg_init();
	if (tinyjpeg_parse_header(jdec, readBuffer, length) < 0)
	{
		LogPrintf("RasterMapManager: JPEG codec error: %s\n", tinyjpeg_get_errorstring(jdec));
		return NULL;
	}

	tinyjpeg_decode(jdec, TINYJPEG_FMT_RGB24);
	tinyjpeg_get_components(jdec, imgComponents);
	tinyjpeg_get_size(jdec, &width, &height);

	return imgComponents[0];
}

unsigned char *RasterMapManager::DecodePNG(unsigned char *readBuffer, unsigned int length, unsigned int& width, unsigned int& height)
{
	static unsigned char* image = 0;
	if (image != 0)
		free(image);

	if (unsigned error = LodePNG_decode32(&image, &width, &height, &readBuffer[0], (size_t)length))
	{
		printf("RasterMapManager: JPEG codec error: %d\n", error);
		return NULL;
	}

	return image;
}
	
} // end namespace OpenGC
