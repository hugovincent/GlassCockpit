/*=========================================================================

  OpenGC - The Open Source Glass Cockpit Project
  Please see our web site at http://www.opengc.org

  Albatross UAV Project - http://www.albatross-uav.org

  Copyright (c) 2001-2003 Damion Shelton
  All rights reserved. Contributions by Hugo Vincent.
  See Copyright.txt or http://www.opengc.org/Copyright.htm for details.

  This software is distributed WITHOUT ANY WARRANTY; without even 
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
  PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include <stdio.h>

#include "Globals.h"
#include "GaugeComponent.h"
#include "SpeedTape.h"

namespace OpenGC
{

SpeedTape::SpeedTape()
{
	m_Font = globals->m_FontManager->LoadDefaultFont();

	m_PhysicalPosition.x = 0;
	m_PhysicalPosition.y = 0;

	m_PhysicalSize.x = 34; // make the clip region larger to handle speed bug
	m_PhysicalSize.y = 136;
	indent_x = m_PhysicalSize.x - 11;

	m_Scale.x = 1.0;
	m_Scale.y = 1.0;
}

SpeedTape::~SpeedTape()
{

}

void SpeedTape::Render()
{
	// Call base class to setup viewport and projection
	GaugeComponent::Render();

	// Speed for floating point calculations
	double airspeed = globals->m_DataSource->GetAirframe()->GetAirspeed_KT();

	// The speed tape doesn't show speeds greater than 1999 knots
	if(airspeed > 1999.0)
		airspeed = 1999.0;

	// Save matrix
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	// Draw in gray-blue
	glColor3ub(51,51,76);

	// Draw the background rectangle
	static const float vertices[] = {
		0.0, 0.0,
		0.0, m_PhysicalSize. y,
		indent_x, m_PhysicalSize.y,
		indent_x, 0.0,
		0.0, 0.0
	};
	glVertexPointer(2, GL_FLOAT, 0, &vertices);
	glDrawArrays(GL_POLYGON, 0, 5);

	// Tick marks are space every 10 kph vertically
	// The tick spacing represents how far apart they are in physical
	// units
	const double tickSpacing = 11.3, tickWidth = 3.7, fontHeight = 5, fontWidth = 4.1, fontIndent = 5;
	const int numTicks = m_PhysicalSize.y / tickSpacing;

	globals->m_FontManager->SetSize(m_Font, fontHeight, fontWidth);

	double nextHighestAirspeed = (((int)airspeed) / 10) * 10;
	if (nextHighestAirspeed < airspeed)
		nextHighestAirspeed += 10;

	// The vertical offset is how high in physical units the next highest 10's
	// airspeed is above the arrow. For airspeeds divisible by 10, this is 0. I.e. 120, 130
	// etc. are all aligned with the arrow
	double vertOffset = (nextHighestAirspeed - airspeed) * tickSpacing / 10.f;

	glColor3ub(255,255,255);
	glLineWidth(2.0);
	
	// Draw ticks up from the center
	for (int i = -numTicks/2; i <= numTicks/2; i++)
	{
		int tickSpeed = nextHighestAirspeed + i * 10;
		double tickLocation = (m_PhysicalSize.y/2) + i * tickSpacing + vertOffset;
		double texty = tickLocation - fontHeight / 2;
		
		if (tickSpeed >= 0)
		{
			const float vertices[] = {indent_x - tickWidth, tickLocation, indent_x, tickLocation};
			glVertexPointer(2, GL_FLOAT, 0, &vertices);
			glDrawArrays(GL_LINES, 0, 2);
			
			if ((tickSpeed % 20) == 0)
			{
				globals->m_FontManager->SetRightAligned(m_Font, true);
				static char buffer[16];
				sprintf( buffer, "%d", tickSpeed);
				globals->m_FontManager->Print(fontIndent + fontWidth*3, texty, &buffer[0], m_Font);
				globals->m_FontManager->SetRightAligned(m_Font, false);
			}
		}
	}
	glPopMatrix();
}

} // end namespace OpenGC
