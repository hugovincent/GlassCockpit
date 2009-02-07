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
	glBegin(GL_POLYGON);
	glVertex2f(0.0, 0.0);
	glVertex2f(0.0, m_PhysicalSize. y);
	glVertex2f(indent_x, m_PhysicalSize.y);
	glVertex2f(indent_x, 0.0);
	glVertex2f(0.0, 0.0);
	glEnd();

	// Tick marks are space every 10 kph vertically
	// The tick spacing represents how far apart they are in physical
	// units
	double tickSpacing = 11.3;
	double tickWidth = 3.7;
	double fontHeight = 5;
	double fontWidth = 4.1;
	double fontIndent = 6;

	globals->m_FontManager->SetSize(m_Font, fontHeight, fontWidth);

	char buffer[1];

	double nextHighestAirspeed = (((int)airspeed) / 10) * 10;
	if (nextHighestAirspeed < airspeed)
		nextHighestAirspeed += 10;

	// The vertical offset is how high in physical units the next highest 10's
	// airspeed is above the arrow. For airspeeds divisible by 10, this is 0. I.e. 120, 130
	// etc. are all aligned with the arrow
	double vertOffset = (nextHighestAirspeed - airspeed) * tickSpacing / 10.f;

	// Vertical location of the tick mark
	double tickLocation = 0;

	glColor3ub(255,255,255);
	glLineWidth(2.0);

	double i = 0; // counter
	int tickSpeed; // speed represented by tick mark
	int charSpd; // speed for computing text digits

	// Draw ticks up from the center
	for (i = 0; i <= ((m_PhysicalSize.y/2) / tickSpacing) + 1; i += 1.0)
	{
		tickSpeed = (int)(nextHighestAirspeed + i * 10.0);
		tickLocation = (m_PhysicalSize.y/2) + i * tickSpacing + vertOffset;

		glBegin(GL_LINES);
		glVertex2f(indent_x - tickWidth, tickLocation);
		glVertex2f(indent_x, tickLocation);
		glEnd();

		if( tickSpeed < 1999.0 && (tickSpeed % 20)==0 )
		{
			double texty = tickLocation - fontHeight / 2;

			charSpd = tickSpeed;

			if(charSpd >= 1000)
			{
				// 1000's
				sprintf( buffer, "%i", charSpd/1000);
				globals->m_FontManager->Print(fontIndent - fontWidth, texty, &buffer[0], m_Font);
				charSpd = charSpd-1000*(int)(charSpd/1000);
			}
			
			if(charSpd >= 100)
			{
				// 100's
				sprintf( buffer, "%i", charSpd/100);
				globals->m_FontManager->Print(fontIndent, texty, &buffer[0], m_Font);
				charSpd = charSpd-100*(int)(charSpd/100);
			}

			// 10's
			if(charSpd >= 10 || tickSpeed > 10)
			{
				sprintf( buffer, "%i", charSpd/10);
				globals->m_FontManager->Print(fontIndent + fontWidth, texty, &buffer[0], m_Font);
				charSpd = charSpd-10*(int)(charSpd/10);
			}

			buffer[0] = '0';
			globals->m_FontManager->Print(fontIndent + fontWidth*2, texty, &buffer[0], m_Font);
		}
	}

	// Draw ticks down from the center
	for (i = 1; i <= ((m_PhysicalSize.y/2) / tickSpacing) + 1; i += 1.0)
	{
		tickSpeed = (int)(nextHighestAirspeed - i * 10);

		// Only draw ticks if they correspond to an airspeed of > 0
		if (tickSpeed < 1999.0 && tickSpeed >= 0)
		{
			tickLocation = (m_PhysicalSize.y/2) - ((i-1) * tickSpacing) - (tickSpacing - vertOffset);

			glBegin(GL_LINES);
			glVertex2f(indent_x - tickWidth, tickLocation);
			glVertex2f(indent_x, tickLocation);
			glEnd();

			if( (tickSpeed % 20)==0 )
			{
				double texty = tickLocation - fontHeight / 2;

				charSpd = tickSpeed;
				
				if(charSpd >= 1000)
				{
					// 1000's
					sprintf( buffer, "%i", charSpd/1000);
					globals->m_FontManager->Print(fontIndent - fontWidth, texty, &buffer[0], m_Font);
					charSpd = charSpd-1000*(int)(charSpd/1000);
				}

				if(charSpd >= 100)
				{
					// 100's
					sprintf( buffer, "%i", charSpd/100);
					globals->m_FontManager->Print(fontIndent, texty, &buffer[0], m_Font);
					charSpd = charSpd-100*(int)(charSpd/100);
				}

				// 10's
				if(charSpd >= 10 || tickSpeed > 10)
				{
					sprintf( buffer, "%i", charSpd/10);
					globals->m_FontManager->Print(fontIndent + fontWidth, texty, &buffer[0], m_Font);
					charSpd = charSpd-10*(int)(charSpd/10);
				}

				buffer[0] = '0';
				globals->m_FontManager->Print(fontIndent + fontWidth*2, texty, &buffer[0], m_Font);
			}
		}
	}
	glPopMatrix();
}

} // end namespace OpenGC