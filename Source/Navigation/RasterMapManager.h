/*
 *  RasterMapManager.h
 *  GlassCockpit
 *
 *  Created by Hugo Vincent on 22/02/09.
 *  Copyright 2009 Hugo Vincent. All rights reserved.
 *
 */

#ifndef RasterMapManager_h
#define RasterMapManager_h

#include <string>
#include "RasterMapTile.h"

namespace OpenGC {

/** This class reads and parses cache sets. For now, only MGMap caches generated
 *  by the MapTileCacher.perl script (in Util/, version 1.35) tested.
 */	
class RasterMapManager
{
public:
	typedef enum {
		RMM_CACHE_MGMAPS = 0
	} CacheFormat;
	
	RasterMapManager();
	
	void SetCachePath(CacheFormat format, const std::string&);
	
	RasterMapTile *GetTile(const unsigned int zoom, const unsigned int x, const unsigned int y);
	
	void SetCacheMemorySize(const unsigned int cacheSizeBytes); // defaults to 2M
	
private:
	unsigned char *RasterMapManager::ReadJPEG(FILE *fp, unsigned int offset, unsigned int length, unsigned int& width, unsigned int& height);
	
	std::string m_CachePath;
};
	
} // end namespace OpenGC

#endif // RasterMapManager_h