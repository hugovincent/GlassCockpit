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

#ifndef RasterMapManager_h
#define RasterMapManager_h

#include <string>
//#include <map>
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
	
	/** Set cache path. MapType should be e.g. "GoogleTer" to suit your cache */
	void SetCachePath(CacheFormat format, const std::string& path, const std::string& mapType);
	
	/** Get tile at specified tile coordinates and zoom level. Note this may be map-type/format specific */
	RasterMapTile *GetTile(const unsigned int zoom, const unsigned int x, const unsigned int y);

	/** Get tile coordinates for the tile containing specified lat/long in a certain zoom level.
	 *  (x, y) are the tile coordinate, (fx, fy) are the fractional coordinate within the tile. */
	void GetTileCoordsForLatLon(unsigned int& x, unsigned int& y, float& fx, float& fy,
								double lat, double lon, const unsigned int zoom);
	
	/** Set the cache of read-in tiles to be at most cacheSizeBytes (default is no cache) */
	void SetCacheMemorySize(const unsigned int cacheSizeBytes);
	
private:
	// Note: returned image pointer will be invalidated after a new image is read
	unsigned char *DecodeJPEG(unsigned char *readBuffer, unsigned int length, unsigned int& width, unsigned int& height);

	// Note: returned image pointer will be invalidated after a new image is read
	unsigned char *DecodePNG(unsigned char *readBuffer, unsigned int length, unsigned int& width, unsigned int& height);

	bool m_Ready;
	
	std::string m_CachePrefix;
	std::string m_MapType;
	
	static const unsigned int READ_BUFFER_SIZE = 64*1024;
	unsigned char m_ReadBuffer[READ_BUFFER_SIZE];
	
//	std::map
};
	
} // end namespace OpenGC

#endif // RasterMapManager_h
