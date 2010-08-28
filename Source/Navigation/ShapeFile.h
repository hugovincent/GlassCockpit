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

#ifndef ShapeFile_h
#define ShapeFile_h

#include <vector>
#include <string>

namespace OpenGC
{

class ShapeFile
{
public:
	ShapeFile(const std::string& fileName);

	void Draw();

private:
	typedef struct { double dX; double dY; } Point2D;
	typedef std::vector<Point2D> LineString2D;
	typedef std::vector<Point2D> Polygon2D;

	float *m_Points; unsigned int m_PointsCount; // holds coordinates of Point Shapefile
	std::vector<LineString2D> m_Lines; // holds coordinates of Line Shapefile
	std::vector<Polygon2D> m_Polygons; // holds coordinates of Polygon Shapefile 

	struct { float minX; float minY; float maxX; float maxY; } m_BoundingBox; // bounding box of shapefile 
};

} // end namespace OpenGC

#endif // ShapeFile_h
