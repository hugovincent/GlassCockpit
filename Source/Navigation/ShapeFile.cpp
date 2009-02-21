/*
 *  ShapeFile.cpp
 *  GlassCockpit
 *
 *  Created by Hugo Vincent on 8/02/09.
 *  Copyright 2009 Hugo Vincent. All rights reserved.
 *
 */

#include "GLHeaders.h"
#include "ShapeFile.h"
#include "shapefil.h"

namespace OpenGC
{

ShapeFile::ShapeFile(const std::string& fileName)
{
    SHPHandle hSHP = SHPOpen(fileName.c_str(), "rb");
    if (hSHP == NULL) return;

	// Get bounding box from Shapefile
	m_BoundingBox.minX = hSHP->adBoundsMin[0];
    m_BoundingBox.minY = hSHP->adBoundsMin[1];
    m_BoundingBox.maxX = hSHP->adBoundsMax[0];
    m_BoundingBox.maxY = hSHP->adBoundsMax[1];
	
	// Point Shapefile
    if (hSHP->nShapeType == SHPT_POINT)
    {
        SHPObject *psShape;
		// FIXME append points?
		m_Points = new float[hSHP->nRecords * 2];
        for (int i = 0; i < hSHP->nRecords; i++)
        {
            psShape = SHPReadObject(hSHP, i);
			m_Points[2 * i]     = psShape->padfX[0];
			m_Points[2 * i + 1] = psShape->padfY[0];
        }
		m_PointsCount = hSHP->nRecords;
    }
	// Line Shapefile
    else if (hSHP->nShapeType == SHPT_ARC)
    {
        SHPObject *psShape;
	    for (int i = 0; i < hSHP->nRecords; i++)
        {
            psShape = SHPReadObject(hSHP, i);
            LineString2D lineString;
            for (int j = 0; j < psShape->nVertices; j++)
            {
				Point2D pt = {psShape->padfX[j], psShape->padfY[j]};
				lineString.push_back(pt);
            }
            m_Lines.push_back(lineString);
        }
    }
 	// Polygon Shapefile
	else if (hSHP->nShapeType == SHPT_POLYGON)
    {
		SHPObject *psShape;
		for (int i = 0; i < hSHP->nRecords; i++)
		{
			psShape = SHPReadObject(hSHP, i);
      		Polygon2D polygon;
			for (int j = 0; j < psShape->nVertices; j++)
			{
				Point2D pt = {psShape->padfX[j], psShape->padfY[j]};
      			polygon.push_back(pt);
			}
			m_Polygons.push_back(polygon);
		}
		
		// FIXME use gluTesselator for solid polygonal objects
		//gluNewTess()
    }
}

/*
void ShapeFile::InitializeGL()
{
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel (GL_FLAT);
	glEnable (GL_LINE_SMOOTH);
	glHint (GL_LINE_SMOOTH_HINT, GL_DONT_CARE);

	glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LINE_SMOOTH);  
	
	//Assign Default Map Bounds to glOrtho
	sBoundingBox.fMaxX=180.0f;
	sBoundingBox.fMaxY=90.0f;
	sBoundingBox.fMinX=-180.0f;
	sBoundingBox.fMinY=-90.0f;
}

void ShapeFile::resizeGL(int w, int h)
{
	if(h<=0) h=1 ;
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	//Assign Bounding Box Coordinates of Shapefile to glOrtho()
	glOrtho(sBoundingBox.fMinX, sBoundingBox.fMaxX,sBoundingBox.fMinY,sBoundingBox.fMaxY,-1,1);
	glMatrixMode(GL_MODELVIEW);
}
*/

void ShapeFile::Draw()
{	
//	glClear (GL_COLOR_BUFFER_BIT);
	glColor3f (0.0, 0.0, 1.0);
	glLoadIdentity ();

	// Render Point Shapefile
	glColor3f (0.0, 0.0, 1.0);
	glEnable(GL_POINT_SMOOTH) ;
	glPointSize(5.0);
	glVertexPointer(2, GL_FLOAT, 0, m_Points);
	glDrawArrays(GL_POINTS, 0, m_PointsCount);
	
	// Render Line Shapefile
	glColor3f (0.0, 1.0, 0.0);
	for (int i = 0; i < m_Lines.size(); i++)
	{
		glBegin(GL_LINE_STRIP);
		for(int j = 0; j < m_Lines[i].size(); j++)
		{
			glVertex2f(m_Lines[i][j].dX, m_Lines[i][j].dY);
		}
		glEnd();
	}
	
	// Render Polygon Shapefile
	glColor3f(1.0,0.0, 0.0);
	for (int i = 0; i < m_Polygons.size(); i++)
	{
		glBegin(GL_LINE_LOOP);
		for(int j = 0; j < m_Polygons[i].size(); j++)
		{
			glVertex2f(m_Polygons[i][j].dX, m_Polygons[i][j].dY);
		}
		glEnd();
	}
	
	glFlush();
}

int main1(int argc, char** argv)
{	
	ShapeFile shape1("Shapefiles/strassen.shp"); // line shapefile
	ShapeFile shape2("Shapefiles/poi.shp"); // point shapefile
	ShapeFile shape3("Shapefiles/gruenflaechen.shp"); // polygon shapefile

	shape1.Draw();
	shape2.Draw();
	shape3.Draw();
	
	return 0;
}

} // end namespace OpenGC
