/*
 *  ShapeFile.h
 *  GlassCockpit
 *
 *  Created by Hugo Vincent on 8/02/09.
 *  Copyright 2009 Hugo Vincent. All rights reserved.
 *
 */

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
