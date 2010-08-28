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

#ifndef RasterMapTile_h
#define RasterMapTile_h

/** Represents one tile in a raster map.
 */

namespace OpenGC {

class RasterMapTile
{
public:
	RasterMapTile(unsigned char *image, unsigned int width, unsigned int height);
	
	unsigned char *Image() { return m_Image; }
	unsigned int Width() { return m_Width; }
	unsigned int Height() { return m_Height; }

private:
	unsigned char *m_Image;
	unsigned int m_Width, m_Height;
};
	
} // end namespace OpenGC

#endif // RasterMapTile_h
