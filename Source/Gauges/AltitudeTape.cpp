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
#include <math.h>

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
	const float vertices[] = {
		0.0, 0.0,
		0.0, m_PhysicalSize. y,
		m_PhysicalSize.x, m_PhysicalSize.y,
		m_PhysicalSize.x, 0.0,
		0.0, 0.0
	};
	glVertexPointer(2, GL_FLOAT, 0, &vertices);
	glDrawArrays(GL_POLYGON, 0, 5);	
	
	// Tick marks are spaced every 100 ft. vertically
	// The tick spacing represents how far apart they are in physical
	// units
	const double tickSpacing = 17.0, tickWidth = 3.7, fontHeight = 4, fontWidth = 3.5, fontIndent = 4.5;
	const int numTicks = m_PhysicalSize.y / tickSpacing;

	globals->m_FontManager->SetSize(m_Font, fontHeight, fontWidth);

	int nextHighestAlt = (alt/100)*100;
	if (nextHighestAlt < alt)
		nextHighestAlt += 100;

	// The vertical offset is how high in physical units the next highest 100's
	// altitude is above the arrow
	double vertOffset = ( (double)nextHighestAlt - (double)alt)/100*tickSpacing;

	glColor3ub(255,255,255);
	glLineWidth(2.0);

	// Draw ticks up from the center
	for (int i = -numTicks/2; i <= numTicks/2; i++)
	{
		int tickAlt = nextHighestAlt + i * 100;
		double tickLocation = (m_PhysicalSize.y/2) + i * tickSpacing + vertOffset;
		double texty = tickLocation - fontHeight / 2;
		
		if (tickAlt >= 0)
		{
			const float vertices[] = {0, tickLocation, tickWidth, tickLocation};
			glVertexPointer(2, GL_FLOAT, 0, &vertices);
			glDrawArrays(GL_LINES, 0, 2);
			
			if ((tickAlt % 200) == 0)
			{
				globals->m_FontManager->SetRightAligned(m_Font, true);
				static char buffer[16];
				sprintf( buffer, "%d", tickAlt);
				globals->m_FontManager->Print(fontIndent + fontWidth * 5, texty, &buffer[0], m_Font);
				globals->m_FontManager->SetRightAligned(m_Font, false);
			}
		}
	}

	glPopMatrix();
}

} // end namespace OpenGC
