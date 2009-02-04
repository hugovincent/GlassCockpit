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
#include "AltitudeTape.h"

namespace OpenGC
{

AltitudeTape::AltitudeTape()
{
	m_Font = globals->m_FontManager->LoadDefaultFont();

	m_PhysicalPosition.x = 0;
	m_PhysicalPosition.y = 0;

	m_PhysicalSize.x = 24;
	m_PhysicalSize.y = 136;

	m_Scale.x = 1.0;
	m_Scale.y = 1.0;
}

AltitudeTape::~AltitudeTape()
{

}

void AltitudeTape::Render()
{
	// Call base class to setup viewport and projection
	GaugeComponent::Render();

	// Get the altitude
	long int alt = (long int) globals->m_DataSource->GetAirframe()->GetAltitude_MSL_Feet();

	// Save matrix
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	// Draw in gray-blue
	glColor3ub(51,51,76);

	// Draw the background rectangle
	glBegin(GL_POLYGON);
	glVertex2f(0.0, 0.0);
	glVertex2f(0.0, m_PhysicalSize. y);
	glVertex2f(m_PhysicalSize.x, m_PhysicalSize.y);
	glVertex2f(m_PhysicalSize.x, 0.0);
	glVertex2f(0.0, 0.0);
	glEnd();

	// Tick marks are spaced every 100 ft. vertically
	// The tick spacing represents how far apart they are in physical
	// units
	double tickSpacing = 17.0;
	double tickWidth = 3.7;
	double fontHeight = 4;
	double fontWidth = 3.5;
	double fontIndent = 4.5;

	globals->m_FontManager->SetSize(m_Font, fontHeight, fontWidth);

	char buffer[1];

	int nextHighestAlt = (alt/100)*100;

	if (nextHighestAlt < alt)
		nextHighestAlt += 100;

	// The vertical offset is how high in physical units the next highest 100's
	// altitude is above the arrow
	double vertOffset = ( (double)nextHighestAlt - (double)alt)/100*tickSpacing;

	// Vertical location of the tick mark
	double tickLocation = 0;

	glColor3ub(255,255,255);
	glLineWidth(2.0);

	double i = 0; // counter
	int tickAlt; // speed represented by tick mark
	int charAlt; // speed for computing text digits

	// Draw ticks up from the center
	for (i = 0; i <= ((m_PhysicalSize.y/2) / tickSpacing) + 1; i += 1.0)
	{
		tickAlt = nextHighestAlt +(int)(i*100.0);
		tickLocation = (m_PhysicalSize.y/2) + i*tickSpacing+vertOffset;
		double texty = tickLocation - fontHeight / 2;

		glBegin(GL_LINES);
		glVertex2f(0, tickLocation);
		glVertex2f(tickWidth, tickLocation);
		glEnd();

		if( (tickAlt % 200)==0 )
		{
			charAlt = tickAlt;
			bool tenk = false;
			bool onek = false;

			// 10000's
			if(charAlt >= 10000)
			{
				//_itoa( charAlt / 10000, buffer, 10);
				sprintf( buffer, "%i", charAlt/10000);
				globals->m_FontManager->Print(fontIndent, texty, buffer, m_Font);
				charAlt = charAlt-10000*(int)(charAlt/10000);

				tenk = true;
			}

			// 1000's
			if(charAlt >= 1000)
			{
				//_itoa( charAlt / 1000, buffer, 10);
				sprintf( buffer, "%i", charAlt/1000);
				globals->m_FontManager->Print(fontIndent + fontWidth, texty, &buffer[0], m_Font);
				charAlt = charAlt-1000*(int)(charAlt/1000);

				onek = true;
			}
			else if(tenk)
			{
				buffer[0] = '0';
				globals->m_FontManager->Print(fontIndent + fontWidth, texty, &buffer[0], m_Font);
			}

			// 100's
			if(charAlt >= 100)
			{
				//_itoa( charAlt / 100, buffer, 10);
				sprintf( buffer, "%i", charAlt/100);
				globals->m_FontManager->Print(fontIndent + fontWidth*2, texty, &buffer[0], m_Font);
				charAlt = charAlt-100*(int)(charAlt/100);
			}
			else if(tenk || onek)
			{
				buffer[0] = '0';
				globals->m_FontManager->Print(fontIndent + fontWidth*2, texty, &buffer[0], m_Font);
			}

			buffer[0] = '0';
			globals->m_FontManager->Print(fontIndent + fontWidth*3, texty, &buffer[0], m_Font);

			buffer[0] = '0';
			globals->m_FontManager->Print(fontIndent + fontWidth*4, texty, &buffer[0], m_Font);
		}
	}

	// Draw ticks down from the center
	for (i = 1; i <= ((m_PhysicalSize.y/2) / tickSpacing) + 1; i += 1.0)
	{
		tickAlt = nextHighestAlt - (int)(i*100);
		tickLocation = (m_PhysicalSize.y/2) - ( (i-1) * tickSpacing) - (tickSpacing - vertOffset);
		double texty = tickLocation - fontHeight / 2;

		if (tickAlt >= 0) {
			glBegin(GL_LINES);
			glVertex2f(0, tickLocation);
			glVertex2f(tickWidth, tickLocation);
			glEnd();
		}

		if( (tickAlt % 200)==0 )
		{
			charAlt = tickAlt;
			bool tenk = false;
			bool onek = false;

			// 10000's
			if(charAlt >= 10000)
			{
				//_itoa( charAlt / 10000, buffer, 10);
				sprintf(buffer, "%i", charAlt/10000);
				globals->m_FontManager->Print(fontIndent, texty, buffer, m_Font);
				charAlt = charAlt-10000*(int)(charAlt/10000);

				tenk = true;
			}

			// 1000's
			if(charAlt >= 1000)
			{
				//_itoa( charAlt / 1000, buffer, 10);
				sprintf(buffer, "%i", charAlt/1000);
				globals->m_FontManager->Print(fontIndent + fontWidth, texty, &buffer[0], m_Font);
				charAlt = charAlt-1000*(int)(charAlt/1000);

				onek = true;
			}
			else if(tenk)
			{
				buffer[0] = '0';
				globals->m_FontManager->Print(fontIndent + fontWidth, texty, &buffer[0], m_Font);
			}

			// 100's
			if(charAlt >= 100)
			{
				//_itoa( charAlt / 100, buffer, 10);
				sprintf(buffer, "%i", charAlt/100);
				globals->m_FontManager->Print(fontIndent + fontWidth*2, texty, &buffer[0], m_Font);
				charAlt = charAlt-100*(int)(charAlt/100);
			}
			else if(tenk || onek)
			{
				buffer[0] = '0';
				globals->m_FontManager->Print(fontIndent + fontWidth*2, texty, &buffer[0], m_Font);
			}

			if(tickAlt > 0) {
				buffer[0] = '0';
				globals->m_FontManager->Print(fontIndent + fontWidth*3, texty, &buffer[0], m_Font);
			}

			if (tickAlt >= 0) {
				buffer[0] = '0';
				globals->m_FontManager->Print(fontIndent + fontWidth*4, texty, &buffer[0], m_Font);
			}
		}
	}

	glPopMatrix();
}

} // end namespace OpenGC
