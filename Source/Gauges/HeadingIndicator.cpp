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

#include "Globals.h"
#include "GLHeaders.h"
#include <stdio.h>
#include <math.h>

#include "GaugeComponent.h"
#include "CircleEvaluator.h"
#include "HeadingIndicator.h"

namespace OpenGC
{

HeadingIndicator::HeadingIndicator()
{
	m_Font = globals->m_FontManager->LoadDefaultFont();

	m_PhysicalPosition.x = 0;
	m_PhysicalPosition.y = 0;

	m_PhysicalSize.x = 130;
	m_PhysicalSize.y = 45;

	m_Scale.x = 1.0;
	m_Scale.y = 1.0;
}

HeadingIndicator::~HeadingIndicator()
{

}

void HeadingIndicator::Render()
{
	// Call base class to setup viewport and projection
	GaugeComponent::Render();

	/**
	 * A few general notes:
	 *
	 * This is a hard gauge to get sized correctly, so we set up a bunch
	 * of symbolic constants for reference rather than hard coding everything.
	 * 
	 * Also, the implementation of the 777 heading indicator is a little curious
	 * because although the gauge is circular in the real world, a given number
	 * of degrees on the displayed circle do not represent an equivalent
	 * difference in heading. indicatorDegreesPerTrueDegrees is the conversion
	 * factor between "gauge" degrees and "display" degrees.
	 */

	const double centerX = 60;
	const double centerY = -35;
	const double radius = 70.0;
	const double indicatorDegreesPerTrueDegrees = 1.5;

	const double bigFontSize = 5.0;
	const double littleFontSize = 3.5;

	char buffer[32];

	glTranslated(centerX, centerY, 0);

	// Draw in gray
	glColor3ub(51,51,76);
	glLineWidth( 1.5 );

	// Set up the circle
	CircleEvaluator aCircle;
	aCircle.SetRadius(radius);
	aCircle.SetArcStartEnd(300,60);
	aCircle.SetDegreesPerPoint(2.5);
	aCircle.SetOrigin(0.0, 0.0);

	// Draw the circle
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(0,0);
	aCircle.Evaluate();
	glEnd();
	glLineWidth(1.0);
	glBegin(GL_LINE_STRIP);
	aCircle.Evaluate(); // and again for antialiasing
	glEnd();

	// Draw the center detent
	glColor3ub(255,255,255);
	glBegin(GL_LINE_LOOP);
	glVertex2f(0.0f,radius);
	glVertex2f(-2.8f,radius+3.25);
	glVertex2f(2.8f,radius+3.25);
	glEnd();

	double heading = globals->m_DataSource->GetAirframe()->GetTrue_Heading();

	// -----------Draw the detents around the circle-----------------------------
	// Figure out the nearest heading that's a multiple of 10
	double nearestTen = (double)( (int)heading - (int)heading % 10);

	// Derotate by this offset
	glRotated((heading - nearestTen) * indicatorDegreesPerTrueDegrees,0,0,1);

	// Now derotate by 40 "virtual" degrees
	glRotated(40*indicatorDegreesPerTrueDegrees,0,0,1);


	// Now draw lines 5 virtual degrees apart around the perimeter of the circle
	for(int i = (int)nearestTen - 40; i<=nearestTen+40; i+=5)
	{
		// The length of the tickmarks on the compass rose
		double tickLength;

		// Make sure the display heading is between 0 and 360
		int displayHeading = (i+720)%360;

		// If the heading is a multiple of ten, it gets a long tick
		if(displayHeading%10==0)
		{
			tickLength = 4;

			// The x-position of the font (depends on the number of characters in the heading)
			double fontx;

			// Convert the display heading to a string
			//_itoa( displayHeading/10, buffer, 10);
			sprintf( buffer, "%d", displayHeading/10 );
			if(displayHeading%30==0)
			{
				if(displayHeading>=100)
					fontx = -bigFontSize;
				else
					fontx = -bigFontSize/2;

				globals->m_FontManager->SetSize(m_Font, bigFontSize, bigFontSize);
				globals->m_FontManager->Print(fontx, radius-tickLength-bigFontSize, &buffer[0], m_Font );
			}
			else
			{
				if(displayHeading>=100)
					fontx = -littleFontSize;
				else
					fontx = -littleFontSize/2;

				globals->m_FontManager->SetSize(m_Font, littleFontSize, littleFontSize);
				globals->m_FontManager->Print(fontx, radius-tickLength-littleFontSize, &buffer[0], m_Font );
			}

		}
		else // Otherwise it gets a short tick
			tickLength = 2.5;

		glBegin(GL_LINES);
		glVertex2f(0,radius);
		glVertex2f(0,radius-tickLength);
		glEnd();

		glRotated(5.0 * indicatorDegreesPerTrueDegrees,0,0,-1);
	}
}

} // end namespace OpenGC

