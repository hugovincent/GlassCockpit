/*
 *  ShapeLibGL.h
 *  GlassCockpit
 *
 *  Created by Hugo Vincent on 8/02/09.
 *  Copyright 2009 Hugo Vincent. All rights reserved.
 *
 */

#include <stdlib.h>
 
#include "shapefil.h"

 
#include <vector>
using namespace std;


typedef struct MyPoint2D
{
	double dX;
	double dY;
} MyPoint2D;
 
//Holds Coordinates of Point Shapefile
vector<MyPoint2D> vPoints;


typedef struct MyLineString2D
{
	vector<MyPoint2D> vPointList;
} MyLineString2D;

//Holds Coordinates of Line Shapefile
vector<MyLineString2D> vLines;


typedef struct MyPolygon2D
{
	vector<MyPoint2D> vPointList;
} MyPolygon2D;

//Holds Coordinates of Polygon Shapefile 
vector<MyPolygon2D> vPolygons;


typedef struct SBoundingBox
{
	float fMaxX;
	float fMaxY;
	float fMinX;
	float fMinY;
} SBoundingBox;

//Bounding Box of Shapefile 
SBoundingBox sBoundingBox;


//Function to Open Shapefile and parse the info 
void OpenShapeFile(char* fileName);
void initializeGL();
void resizeGL(int w, int h);
void draw(); 
