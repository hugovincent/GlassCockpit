/*=========================================================================

  OpenGC - The Open Source Glass Cockpit Project
  Please see our web site at http://www.opengc.org

  Albatross UAV Project - http://www.albatross-uav.org

  Copyright (c) 2006 Hugo Vincent
  All rights reserved.
  See Copyright.txt or http://www.opengc.org/Copyright.htm for details.

  This software is distributed WITHOUT ANY WARRANTY; without even 
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
  PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "Globals.h"
#include "FlightDirector.h"

/** 
 * This draws all of the Flight Director except for the artificial 
 * horizon roll/pitch indicator. That is: the flight director altitude,
 * airspeed and heading.
 *
 * It is kind of messy to put this all in here (as oppose to in each
 * respective GaugeComponent, because information about the layout of the 
 * GaugeComponents in PFD.cpp needs to be hardcoded in here. 
 * But it does keep it all together, and keeps the other GaugeComponents
 * as simple as possible.
 */

namespace OpenGC
{

FlightDirector::FlightDirector()
{
	m_Font = globals->m_FontManager->LoadDefaultFont();
	this->SetOpaque(false); // this is overlaid so we don't want an opaque background
	
	m_PhysicalSize.x = 200;
	m_PhysicalSize.y = 190;

	m_Scale.x = 1.0;
	m_Scale.y = 1.0;
}

FlightDirector::~FlightDirector()
{

}

void FlightDirector::Render()
{
	GaugeComponent::Render();

	AirframeDataContainer *data = globals->m_DataSource->GetAirframe();

	const int ALT_X = 149;
	const int ALT_Y = 32;
	const int ALT_HEIGHT = 135;
	const int AIRSPEED_X = 40;
	const int AIRSPEED_Y = 32;
	const int AIRSPEED_HEIGHT = 135;

	if (data->GetDirector_Active())
	{
		glColor3ub(255, 0, 255); // magenta
		glLineWidth(2.0);
		
		// Get the data
		double fdAlt = globals->m_DataSource->GetAirframe()->GetDirector_Altitude();
		double alt = globals->m_DataSource->GetAirframe()->GetAltitude_MSL_Feet();
		double fdAirspeed = globals->m_DataSource->GetAirframe()->GetDirector_Airspeed();
		double airspeed = globals->m_DataSource->GetAirframe()->GetAirspeed_KT();
		double heading = globals->m_DataSource->GetAirframe()->GetTrue_Heading();
		double fdHeading = globals->m_DataSource->GetAirframe()->GetDirector_Heading();

		///////////////////////////////////////////////////////////////////////
		// Altitude marker

		// Is the altitude on screen?
		double deltaAlt = alt - fdAlt;
		if (deltaAlt < 400.0 && deltaAlt > -400.0) {
			double markerOffset = 61.0 - (deltaAlt / 100.0 * 17.0);
			float vertices[] = {
				ALT_X - 1, ALT_Y + markerOffset + 7.0,
				ALT_X - 4, ALT_Y + markerOffset + 4.0,
				ALT_X - 4, ALT_Y + markerOffset + 10.0
			};
			glVertexPointer(2, GL_FLOAT, 0, &vertices);
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}
		// It's not visible, so draw an arrow at the edge of the window
		// pointing in the correct direction.
		else if (deltaAlt < 400.0) { // top
			float vertices[] = {
				ALT_X - 4.0, ALT_Y + ALT_HEIGHT,
				ALT_X - 1.0, ALT_Y + ALT_HEIGHT - 3.0,
				ALT_X - 2.7, ALT_Y + ALT_HEIGHT - 3.0,
				ALT_X - 2.7, ALT_Y + ALT_HEIGHT - 6.0,
				ALT_X - 5.3, ALT_Y + ALT_HEIGHT - 6.0,
				ALT_X - 5.3, ALT_Y + ALT_HEIGHT - 3.0,
				ALT_X - 7.0, ALT_Y + ALT_HEIGHT - 3.0
			};
			glVertexPointer(2, GL_FLOAT, 0, &vertices);
			glDrawArrays(GL_LINE_LOOP, 0, 7);
		} 
		else { // bottom
			float vertices[] = {
				ALT_X - 4.0, ALT_Y,
				ALT_X - 1.0, ALT_Y + 3.0,
				ALT_X - 2.7, ALT_Y + 3.0,
				ALT_X - 2.7, ALT_Y + 6.0,
				ALT_X - 5.3, ALT_Y + 6.0,
				ALT_X - 5.3, ALT_Y + 3.0,
				ALT_X - 7.0, ALT_Y + 3.0
			};
			glVertexPointer(2, GL_FLOAT, 0, &vertices);
			glDrawArrays(GL_LINE_LOOP, 0, 7);
		}

		///////////////////////////////////////////////////////////////////////
		// Airspeed marker

		double deltaAirspeed = airspeed - fdAirspeed;

		// Is the airspeed on screen?
		if (deltaAirspeed < 55.0 && deltaAirspeed > -55.0) {
			double markerOffset = 62.0 - (deltaAirspeed / 10.0 * 11.3);
			float vertices[] = {
				AIRSPEED_X - 7, AIRSPEED_Y + markerOffset + 7.0,
				AIRSPEED_X - 4, AIRSPEED_Y + markerOffset + 4.0,
				AIRSPEED_X - 4, AIRSPEED_Y + markerOffset + 10.0
			};
			glVertexPointer(2, GL_FLOAT, 0, &vertices);
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}
		// It's not visible, so draw an arrow at the edge of the window
		// pointing in the correct direction.
		else if (deltaAirspeed < 55.0) { // top
			float vertices[] = {
				AIRSPEED_X - 4.0, AIRSPEED_Y + AIRSPEED_HEIGHT,
				AIRSPEED_X - 1.0, AIRSPEED_Y + AIRSPEED_HEIGHT - 3.0,
				AIRSPEED_X - 2.7, AIRSPEED_Y + AIRSPEED_HEIGHT - 3.0,
				AIRSPEED_X - 2.7, AIRSPEED_Y + AIRSPEED_HEIGHT - 6.0,
				AIRSPEED_X - 5.3, AIRSPEED_Y + AIRSPEED_HEIGHT - 6.0,
				AIRSPEED_X - 5.3, AIRSPEED_Y + AIRSPEED_HEIGHT - 3.0,
				AIRSPEED_X - 7.0, AIRSPEED_Y + AIRSPEED_HEIGHT - 3.0
			};
			glVertexPointer(2, GL_FLOAT, 0, &vertices);
			glDrawArrays(GL_LINE_LOOP, 0, 7);
		} 
		else { // bottom
			float vertices[] = {
				AIRSPEED_X - 4.0, AIRSPEED_Y,
				AIRSPEED_X - 1.0, AIRSPEED_Y + 3.0,
				AIRSPEED_X - 2.7, AIRSPEED_Y + 3.0,
				AIRSPEED_X - 2.7, AIRSPEED_Y + 6.0,
				AIRSPEED_X - 5.3, AIRSPEED_Y + 6.0,
				AIRSPEED_X - 5.3, AIRSPEED_Y + 3.0,
				AIRSPEED_X - 7.0, AIRSPEED_Y + 3.0
			};
			glVertexPointer(2, GL_FLOAT, 0, &vertices);
			glDrawArrays(GL_LINE_LOOP, 0, 7);
		}

		///////////////////////////////////////////////////////////////////////
		// Heading marker
		
		glPushMatrix();
		glTranslated(89, -30, 0); // Center of the Heading Indicator
		const double radius = 70.0; // Radius of Heading Indicator
		const double indicatorDegreesPerTrueDegrees = 1.5;

		// Is the heading on screen?
		double deltaHeading = heading - fdHeading;
		if (deltaHeading < 30.0 || deltaHeading > 330.0) {
			// It's visible so draw the diamond-shaped marker
			if (deltaHeading < 180.0)
				glRotated(deltaHeading * indicatorDegreesPerTrueDegrees, 0,0,1);
			else
				glRotated((360.0 - deltaHeading) * indicatorDegreesPerTrueDegrees, 0,0,-1);
			float vertices[] = {0,radius+4.0,   3,radius+7.0,   -3,radius+7.0};
			glVertexPointer(2, GL_FLOAT, 0, &vertices);
			glDrawArrays(GL_TRIANGLES, 0, 3);
			glLineWidth(1.0);
			glVertexPointer(2, GL_FLOAT, 0, &vertices);
			glDrawArrays(GL_LINE_STRIP, 0, 3);
		}
		else {
			// It's not visible, so draw an arrow at the edge of the window
			// pointing around the heading indicator in the correct direction.
			if (deltaHeading < 180.0) { // right
				glRotated(30.0 * indicatorDegreesPerTrueDegrees, 0,0,1);
				float vertices[] = {0,radius+6,   3,radius+9,   3,radius+7.3,
					6,radius+7.3,   6,radius+4.7,   3,radius+4.7,   3,radius+3};
				glVertexPointer(2, GL_FLOAT, 0, &vertices);
				glDrawArrays(GL_LINE_LOOP, 0, 7);
			} 
			else { // left
				glRotated(30.0 * indicatorDegreesPerTrueDegrees, 0,0,-1);
				float vertices[] = {0,radius+6,   -3,radius+9,   -3,radius+7.3,
					-6,radius+7.3,   -6,radius+4.7,   -3,radius+4.7,   -3,radius+3};
				glVertexPointer(2, GL_FLOAT, 0, &vertices);
				glDrawArrays(GL_LINE_LOOP, 0, 7);
			}
		}
		glPopMatrix();
		
		///////////////////////////////////////////////////////////////////////
		// FD Set-point Text
		char buffer[12];

		sprintf(buffer, "%.0f", fdAlt);
		globals->m_FontManager->SetSize(m_Font, 4, 4);
		globals->m_FontManager->Print(150,170, buffer, m_Font);
		
		sprintf(buffer, "%.1f", fdAirspeed);
		globals->m_FontManager->SetRightAligned(m_Font, true);
		globals->m_FontManager->Print(32,170, buffer, m_Font);
		
		sprintf(buffer, "%3d", (int) fdHeading);
		globals->m_FontManager->Print(100, 13, &buffer[0], m_Font); 
		globals->m_FontManager->SetRightAligned(m_Font, false);
		globals->m_FontManager->Print(75, 13, "FD", m_Font); 
	}
}

} // end namespace OpenGC

