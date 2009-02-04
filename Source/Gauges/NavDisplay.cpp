/*=========================================================================

  OpenGC - The Open Source Glass Cockpit Project
  Please see our web site at http://www.opengc.org

  Albatross UAV Project - http://www.albatross-uav.org

  Copyright (c) 2001-2003 Hugo Vincent
  All rights reserved. Contributions by Damion Shelton.
  See Copyright.txt or http://www.opengc.org/Copyright.htm for details.

  This software is distributed WITHOUT ANY WARRANTY; without even 
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
  PURPOSE.  See the above copyright notice for more information.

 =========================================================================*/

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <string>

#include "Globals.h"
#include "Constants.h"
#include "CircleEvaluator.h"
#include "DataSource.h"
#include "NavDisplay.h"
#include "GeographicObject.h"
#include "WaypointGeoObj.h"
#include "AirportGeoObj.h"
#include "NavaidGeoObj.h"

namespace OpenGC
{

NavDisplay::NavDisplay()
{
	//this->SetGaugeOutline(true);

	m_Font = globals->m_FontManager->LoadDefaultFont();

	m_PhysicalPosition.x = 0;
	m_PhysicalPosition.y = 0;

	m_PhysicalSize.x = 181; // really 180, but give extra space so there is no nasty
	m_PhysicalSize.y = 262; // edge visible at the right hand edge of the window.

	m_Scale.y = 1.0;
	m_Scale.x = 1.0;

	// Set initial gauge size in nautical miles per side
	m_SizeNM = 45.0;
}

NavDisplay::~NavDisplay()
{
	// Destruction handled by base class
}

void NavDisplay::Render()
{
	Gauge::Render();

	///////////////////////////////////////////////////////////////////////////
	// Get the data, and convert to mercator coordinates
	///////////////////////////////////////////////////////////////////////////
	
	// Get the heading
	aircraftHeading = globals->m_DataSource->GetAirframe()->GetTrue_Heading();

	// Where is the aircraft?
	aircraftLat = globals->m_DataSource->GetAirframe()->GetLatitude();
	aircraftLon = globals->m_DataSource->GetAirframe()->GetLongitude();

	bool isInSouthernHemisphere = false;
	if(aircraftLat < 0)
	{
		isInSouthernHemisphere = true;
		aircraftLat = aircraftLat * -1;
	}
	
	// Northing and Easting in nautical miles (Mercator Coordinates)
	GeographicObject::LatLonToMercator(aircraftLat, aircraftLon, 
			mercatorNorthing, mercatorEasting);
	mercatorNorthing /= METER_PER_NM;
	mercatorEasting /= METER_PER_NM;

	if(isInSouthernHemisphere)
		mercatorNorthing = mercatorNorthing * -1.0;

	glMatrixMode(GL_MODELVIEW);
	
	///////////////////////////////////////////////////////////////////////////
	// Draw Layer 0: the map
	///////////////////////////////////////////////////////////////////////////
	
	glPushMatrix();
		glTranslatef(CENTER_X, CENTER_Y, 0.0);
		glRotatef(aircraftHeading, 0, 0, 1);

		PlotMap();
	glPopMatrix();
	
	///////////////////////////////////////////////////////////////////////////
	// Draw Layer 1: the range circles and marks
	///////////////////////////////////////////////////////////////////////////
	
	glColor3ub(255, 255, 255);
	glLineWidth(1.5);
	
	// Range circles
	CircleEvaluator aCircle;
	aCircle.SetDegreesPerPoint(5.0);
	aCircle.SetRadius(40.0);
	aCircle.SetOrigin(CENTER_X, CENTER_Y);
	aCircle.SetArcStartEnd(0.0, 360.0);
	
	glBegin(GL_LINE_STRIP);
	aCircle.Evaluate();
	glEnd();
	
	aCircle.SetRadius(80.0);
	aCircle.SetArcStartEnd(0.0, 360.0);
	glBegin(GL_LINE_STRIP);
	aCircle.Evaluate();
	glEnd();
	
	aCircle.SetRadius(120.0);
	aCircle.SetArcStartEnd(300.0, 60.0);
	glBegin(GL_LINE_STRIP);
	aCircle.Evaluate();
	glEnd();
	
	aCircle.SetRadius(130.0);
	aCircle.SetArcStartEnd(300.0, 60.0);
	glBegin(GL_LINE_STRIP);
	aCircle.Evaluate();
	glEnd();
	
	///////////////////////////////////////////////////////////////////////////
	// Draw Layer 2: the Point of Interest marker (Navaids, Airports)
	///////////////////////////////////////////////////////////////////////////

	// Rotate graphics context about the heading
	glPushMatrix();
		glTranslatef(CENTER_X, CENTER_Y, 0.0);
		glRotatef(aircraftHeading, 0, 0, 1);

		PlotCourse();
		
		// Naviads (blue)
		glColor3f(0.0, 0.0, 1.0);
		PlotGeoObjs<0>();
		
		// Airports (red)
		glColor3f(1.0, 0.0, 0.0);
		PlotGeoObjs<1>();
		
		PlotWaypoints();
	glPopMatrix();
	
	///////////////////////////////////////////////////////////////////////////
	// Draw Layer 4: Compass Markings
	///////////////////////////////////////////////////////////////////////////

	// Compass markings
	glPushMatrix();
		glColor3ub(255, 255, 255);
		glLineWidth(1.5);
		
		double heading_offset = fmod(aircraftHeading, compass_interval);
		glTranslatef(CENTER_X, CENTER_Y, 0.0);
		glRotatef(heading_offset+50, 0, 0, 1);
		
		for (int i = -50; i < 50; i += compass_interval)
		{
			glRotatef(compass_interval, 0, 0, -1);
			glBegin(GL_LINES);
				glVertex2f(0.0, 120.0);
				glVertex2f(0.0, 130.0);
			glEnd();
		}
	glPopMatrix();
	
	///////////////////////////////////////////////////////////////////////////
	// Draw Layer 5: Textual data, zoom buttons, aircraft icon, wind est.
	///////////////////////////////////////////////////////////////////////////
	
	// Black background
	glColor4f(0.0, 0.0, 0.0, 0.8); // 80% alpha
	glBegin(GL_POLYGON); // behind zoom buttons
		glVertex2f(0.0, OVERLAY_Y);
		glVertex2f(0.0, OVERLAY_Y+20.5);
		glVertex2f(10.5, OVERLAY_Y+20.5);
		glVertex2f(10.5, OVERLAY_Y);
	glEnd();
	glBegin(GL_POLYGON); // behind main overlay
		glVertex2f(0.0, 0.0);
		glVertex2f(0.0, OVERLAY_Y+0.5);
		glVertex2f(m_PhysicalSize.x, OVERLAY_Y+0.5);
		glVertex2f(m_PhysicalSize.x, 0.0);
	glEnd();

	// Lines dividing up the space in the gauge
	glColor3f(0.7, 0.7, 0.7);
	glLineWidth(1.0);
	glBegin(GL_LINES);
		// lines around the zoom buttons
		glVertex2f(0.0,  OVERLAY_Y+10.5);
		glVertex2f(10.5, OVERLAY_Y+10.5);
		glVertex2f(10.5, OVERLAY_Y+0.5);
		glVertex2f(10.5, OVERLAY_Y+20.5);
		glVertex2f(0.0,  OVERLAY_Y+20.5);
		glVertex2f(10.5, OVERLAY_Y+20.5);
		glVertex2f(10.5, OVERLAY_Y+0.5);
		glVertex2f(10.5, OVERLAY_Y+20.5);
		// line along top edge of overlay
		glVertex2f(0.0, OVERLAY_Y+0.5);
		glVertex2f(m_PhysicalSize.x, OVERLAY_Y+0.5);
		// insert more here...
	glEnd();

	// Zoom +/- buttons
	glLineWidth(2.0);
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINES);
		// Plus symbol
		glVertex2f(2.5, OVERLAY_Y+15.0);
		glVertex2f(7.5, OVERLAY_Y+15.0);
		glVertex2f(5.0, OVERLAY_Y+12.5);
		glVertex2f(5.0, OVERLAY_Y+17.5);
		// Minus symbol
		glVertex2f(2.5, OVERLAY_Y+5.0);
		glVertex2f(7.5, OVERLAY_Y+5.0);
	glEnd();

	// Ground speed and track text
	char buffer[20];
	globals->m_FontManager->SetSize(m_Font, 5.0, 5.0);
	sprintf(buffer, "TRACK %.0f", globals->m_DataSource->GetAirframe()->GetTrue_Heading() );
	globals->m_FontManager->Print(2.0, 12.5, &buffer[0], m_Font);
	sprintf(buffer, "GS %.0f", globals->m_DataSource->GetAirframe()->GetGround_Speed_MS() );
	globals->m_FontManager->Print(2.0, 2.5, &buffer[0], m_Font);

	// Scale text
	sprintf(buffer, "Scale: %.1f nm", m_SizeNM / (180.0 / 40.0));
	globals->m_FontManager->SetSize(m_Font, 3.0, 3.0);
	globals->m_FontManager->Print(5.0, OVERLAY_Y+175.0, &buffer[0], m_Font);

	// Wind speed/direction indicator
	PlotWindSpeedDirection();

	// Aircraft symbol
	glColor3ub(255,255,255);
	glLineWidth(2.0);
	const float width = 1.0;
	glBegin(GL_LINES);
		glVertex2f(CENTER_X-width, CENTER_Y-6);
		glVertex2f(CENTER_X-width, CENTER_Y+6);
		glVertex2f(CENTER_X+width, CENTER_Y-6);
		glVertex2f(CENTER_X+width, CENTER_Y+6);
		glVertex2f(CENTER_X-4,     CENTER_Y-6);
		glVertex2f(CENTER_X-width, CENTER_Y-6);
		glVertex2f(CENTER_X-8,     CENTER_Y);
		glVertex2f(CENTER_X-width, CENTER_Y);
		glVertex2f(CENTER_X+4,     CENTER_Y-6);
		glVertex2f(CENTER_X+width, CENTER_Y-6);
		glVertex2f(CENTER_X+8,     CENTER_Y);
		glVertex2f(CENTER_X+width, CENTER_Y);
	glEnd();
	
	// Divider between the PFD and the Nav Display
   	glLineWidth(2.0);
   	glColor3ub(0, 190, 190); // cyan
   	glBegin(GL_LINES);
		glVertex2f(0.0, m_PhysicalSize.y);
		glVertex2f(0.0, 0.0);
   	glEnd();
}

/** Mouse event handler for zoom in/out buttons. */
void NavDisplay::OnMouseDown(int button, double physicalX, double physicalY)
{
	if ((physicalX <= 10.0)&&(physicalY>OVERLAY_Y)&&(physicalY<(OVERLAY_Y+10.0)))
	{
		m_SizeNM *= 2.0;
	}
	else if ((physicalX <= 10.0)&&(physicalY>(OVERLAY_Y+10.0))&&(physicalY<(OVERLAY_Y+20.0)))
	{
		m_SizeNM /= 2.0;
	}  
}

void NavDisplay::PlotWindSpeedDirection()
{
	// Get the data
	//double windSpeed = globals->m_DataSource->GetAirframe()->GetWind_Speed();
	double windDirection = globals->m_DataSource->GetAirframe()->GetWind_Direction();
	
	glPushMatrix();
		glTranslatef(170.0, OVERLAY_Y+10.0, 0.0);
		// Draw the text
		// FIXME

		// Draw the Arrow
		glRotatef(windDirection, 0, 0, 1);

		glLineWidth(1.5);
		glColor3f(0.0, 1.0, 1.0); // sky blue
		glBegin(GL_LINES);
			glVertex2f(0.0, 4.0);
			glVertex2f(0.0, -4.0);
			glVertex2f(-2.0, 2.0);
			glVertex2f(0.0, 4.0);
			glVertex2f(2.0, 2.0);
			glVertex2f(0.0, 4.0);
		glEnd();
	glPopMatrix();
}

void NavDisplay::PlotWaypoints()
{
	// Get the waypoint list
	WaypointList *waypointList = globals->m_NavDatabase->GetWaypointList();

	WaypointList::iterator iter;
	for (iter = waypointList->begin(); iter != waypointList->end(); ++iter)
	{
		int FIXME = 1;
		// FIXME plot the waypoints and planned course
	}
}

void NavDisplay::PlotCourse()
{
	// Get the vector of CoursePoints
	FlightCourse *course = globals->m_NavDatabase->GetFlightCourse();
	FlightCourse::iterator iter;
	
	double xPos, yPos, lat, lon, northing, easting;
	glColor3f(0.5, 0.0, 0.0); // 50% red
	glLineWidth(1.0);
	glBegin(GL_LINE_STRIP);
	for (iter = course->begin(); iter != course->end(); ++iter)
	{
		(*iter).GetCoords(lat, lon);
		GeographicObject::LatLonToMercator(lat, lon, northing, easting);
		PointToPixelCoord(northing, easting, xPos, yPos);
		glVertex2f(xPos, yPos);
	}
	glEnd();
}

template<int i> void NavDisplay::PlotGeoObjs()
{
	// Get the Navaid list bins from the hash
	std::list<GeographicObject*> *geoList;
	if (i == 0)
		geoList = globals->m_NavDatabase->GetNavaidHash()->GetListAtLatLon(aircraftLat, aircraftLon);
	else if (i == 1)
		geoList = globals->m_NavDatabase->GetAirportHash()->GetListAtLatLon(aircraftLat, aircraftLon);

	std::list<GeographicObject*>::iterator iter;
	double objNorthing, objEasting;

	// Point drawing parameters
	glPointSize(2.0);

	for (iter = geoList->begin(); iter != geoList->end(); ++iter)
	{
		// Get mercator coordinates in meters
		(*iter)->GetMercatorMeters(objNorthing, objEasting);

		// Compute position relative to gauge center
		double xPos, yPos;
		PointToPixelCoord(objNorthing, objEasting, xPos, yPos);

		// Only draw the navaid if it's visible within the rendering area
		if (PixelCoordIsVisible(xPos, yPos))
		{
			// We're going to be clever and "derotate" each label
			// by the heading, so that everything appears upright despite
			// the global rotation. This makes all of the labels appear at
			// 0,0 in the local coordinate system
			glPushMatrix();
				glTranslatef(xPos, yPos, 0.0);
				glRotatef(-1.0 * aircraftHeading, 0, 0, 1);

				glBegin(GL_POLYGON);
					glVertex2f(-1.0, 0.0);
					glVertex2f(0.0, 1.0);
					glVertex2f(1.0, 0.0);
					glVertex2f(0.0, -1.0);
				glEnd();

				globals->m_FontManager->SetSize(m_Font, 4.0, 4.0);
				globals->m_FontManager->Print(1.5, -2.0, (*iter)->GetIdentification().c_str(), m_Font);
			glPopMatrix();
		}
	}
}

void NavDisplay::PlotMap()
{
#if 0
	// FIXME actually plot some kind of map!! For now, random points:
	const double points[][2] = {
		{1000, 1000},
		{2000, 2000},
		{1000, 2000},
		{2500, 2500},
		{700, 2800},
		{-1000, 3500},
		{-3000, 0},
		{-5000, 5000}
	};
	
	//double xPos, yPos;
	glColor3f(0.0, 0.5, 0.0); // 50% green
	glBegin(GL_TRIANGLE_STRIP);
		for (int i = 0; i < (int)(sizeof(points) / sizeof(double[2])); i++)
		{
			//PointToPixelCoord(points[i][0], points[i][1], xPos, yPos);
			//glVertex2f(xPos, yPos);
			glVertex2f(points[i][0] / m_SizeNM, points[i][1] / m_SizeNM); // FIXME remove this and uncomment the two above lines
		}
	glEnd();
#endif
}

void NavDisplay::PointToPixelCoord(double objNorthing, double objEasting, double &xPos, double &yPos)
{
	// Compute position relative to gauge center
	yPos = (objNorthing / METER_PER_NM - mercatorNorthing) * 180.0 / m_SizeNM;
	xPos = (objEasting / METER_PER_NM - mercatorEasting) * 180.0 / m_SizeNM;
}

bool NavDisplay::PixelCoordIsVisible(double xPos, double yPos)
{
	return (
		(fabs(xPos) < 90.0) && (yPos < (m_PhysicalSize.y - CENTER_Y)) 
		&& (yPos > (-1 * CENTER_Y)) && (sqrt(yPos*yPos + xPos*xPos) < 120)
		// FIXME - doesn't work correctly
	) ? true : false;
}

} // end namespace OpenGC

