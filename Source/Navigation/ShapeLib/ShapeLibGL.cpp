/*
 *  ShapeLibGL.cpp
 *  GlassCockpit
 *
 *  Created by Hugo Vincent on 8/02/09.
 *  Copyright 2009 Hugo Vincent. All rights reserved.
 *
 */

#include "GLHeaders.h"

#include "ShapeLibGL.h"

//Function to Open Shapefile and parse the info 
void OpenShapeFile(char* fileName)
{
    SHPHandle hSHP=SHPOpen(fileName, "rb" );
	
	//Read Bounding Box of Shapefile
    sBoundingBox.fMaxX=hSHP->adBoundsMax[0];
    sBoundingBox.fMaxY=hSHP->adBoundsMax[1];
	
    sBoundingBox.fMinX=hSHP->adBoundsMin[0];
    sBoundingBox.fMinY=hSHP->adBoundsMin[1];
	
	
    if(hSHP == NULL) return;
	
	//Point Shapefile
    if(hSHP->nShapeType == SHPT_POINT)
    {
        SHPObject *psShape;
        for(int i=0;i<hSHP->nRecords;i++)
        {
            psShape = SHPReadObject(hSHP, i);
			
            double fX = psShape->padfX[0];
            double fY = -psShape->padfY[0];
			
            //Plot these points
            MyPoint2D pt;
            pt.dX=fX;
            pt.dY=-fY;
            vPoints.push_back(pt);
        }
    }
	
	
	//Line Shapefile
    else if(hSHP->nShapeType == SHPT_ARC)
    {
        SHPObject *psShape;
	    for(int i=0;i<hSHP->nRecords;i++)
        {
            psShape = SHPReadObject(hSHP, i);
            vector<MyPoint2D> tempPointArray;
			
            for(int j=0;j<psShape->nVertices;j++)
            {
				double fX = psShape->padfX[j];
				double fY = psShape->padfY[j];
				MyPoint2D pt;
				pt.dX=fX;
				pt.dY=fY;
				tempPointArray.push_back(pt);
            }
			
			MyLineString2D linestring;
            linestring.vPointList=tempPointArray;
            vLines.push_back(linestring);
			
        }
    }
	
 	//Polygon Shapefile
    if(hSHP->nShapeType == SHPT_POLYGON)
    {
		SHPObject *psShape;
		for(int i=0;i<hSHP->nRecords;i++)
		{
			psShape = SHPReadObject(hSHP, i);
      		vector<MyPoint2D> tempPointArray;
			
			for(int j=0;j<psShape->nVertices;j++)
			{
				double fX = psShape->padfX[j];
				double fY = psShape->padfY[j];
				MyPoint2D pt;
				pt.dX=fX;
				pt.dY=fY;
      			tempPointArray.push_back(pt);
			}
			MyPolygon2D polygon;
			polygon.vPointList=tempPointArray;
			vPolygons.push_back(polygon);
		}
		
    }
	
}


void InitializeGL()
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


void resizeGL(int w, int h)
{
	if(h<=0) h=1 ;
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	//Assign Bounding Box Coordinates of Shapefile to glOrtho()
	glOrtho(sBoundingBox.fMinX, sBoundingBox.fMaxX,sBoundingBox.fMinY,sBoundingBox.fMaxY,-1,1);
	glMatrixMode(GL_MODELVIEW);
}


void draw()
{
	
//	glClear (GL_COLOR_BUFFER_BIT);
	glColor3f (0.0, 0.0, 1.0);
	glLoadIdentity ();
	
	//Render Point Shapefile
	glColor3f (0.0, 0.0, 1.0);
	glEnable(GL_POINT_SMOOTH) ;
	glPointSize(5.0);
	glBegin(GL_POINTS);
	
	for(int i=0; i<vPoints.size(); i++)
	{
		glVertex2f(vPoints[i].dX,vPoints[i].dY);
	}
	
	glEnd();
	
	//Render Line Shapefile
	glColor3f (0.0, 1.0, 0.0);
	for(int i=0;i<vLines.size();i++)
	{
		
		glBegin(GL_LINE_STRIP);
		for(int j=0;j<vLines[i].vPointList.size();j++)
		{
			glVertex2f(vLines[i].vPointList[j].dX,vLines[i].vPointList[j].dY);
			
		}
		
		glEnd();
	}
	
	//Render Polygon Shapefile
	glColor3f(1.0,0.0, 0.0);
	for(int i=0;i<vPolygons.size();i++)
	{
		glBegin(GL_LINE_LOOP);
		for(int j=0;j<vPolygons[i].vPointList.size();j++)
		{
			glVertex2f(vPolygons[i].vPointList[j].dX,vPolygons[i].vPointList[j].dY);
		}
		
		glEnd();
	}
	
	glFlush();
}

int main1(int argc, char** argv)
{
//	glutInit(&argc, argv);
//	glutCreateWindow ("Render Shapefiles");
	initializeGL();
	
	OpenShapeFile("Shapefiles\\strassen.shp");//Line Shapefile
	OpenShapeFile("Shapefiles\\poi.shp");//Point Shapefile
	OpenShapeFile("Shapefiles\\gruenflaechen.shp");//Polygon Shapefile
	
//	glutReshapeFunc (resizeGL);
//	glutDisplayFunc (paintGL);
//	glutMainLoop();
	return 0;
}