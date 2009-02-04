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
#include "VSI.h"

namespace OpenGC
{

VSI::VSI()
{
	m_Font = globals->m_FontManager->LoadDefaultFont();

	m_PhysicalPosition.x = 0;
	m_PhysicalPosition.y = 0;

	m_PhysicalSize.x = 16;
	m_PhysicalSize.y = 108;

	m_Scale.x = 1.0;
	m_Scale.y = 1.0;

	m_NeedleCenter = 54.0;
	m_MaxNeedleDeflection = 42.0;
}

VSI::~VSI()
{

}

void VSI::Render()
{
	// Call base class to setup viewport and projection
	GaugeComponent::Render();

	// Save matrix
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	globals->m_FontManager->SetSize(m_Font,4.0, 4.0);

	// Draw in gray-blue
	glColor3ub(51,51,76);

	// Bottom angular part
	glBegin(GL_POLYGON);
	glVertex2f(0.0, 10.0);
	glVertex2f(0.0, 40.0);
	glVertex2f(4.5, 44.0);
	glVertex2f(16.0, 44.0);
	glVertex2f(16.0, 27.0);
	glVertex2f(7.0, 10.0);
	glEnd();

	// Center rectangle
	glBegin(GL_POLYGON);
	glVertex2f(4.5, 44.0);
	glVertex2f(4.5, 66.0);
	glVertex2f(16.0, 66.0);
	glVertex2f(16.0, 44.0);
	glEnd();

	// Top angular part
	glBegin(GL_POLYGON);
	glVertex2f(4.5, 66.0);
	glVertex2f(0.0, 70.0);
	glVertex2f(0.0, 100.0);
	glVertex2f(7.0, 100.0);
	glVertex2f(16.0, 83.0);
	glVertex2f(16.0, 66.0);
	glEnd();

	glColor3ub( 255, 255, 255 );

	double Vertical_Speed_FPM = globals->m_DataSource->GetAirframe()->GetVertical_Speed_FPM();

	char buffer[5];

	// if vertical +- 200 fpm display digital readout
	if ( Vertical_Speed_FPM < -19.0 )
	{
		sprintf(buffer, "%04d", (int) Vertical_Speed_FPM);
		globals->m_FontManager->Print( 1.0, 3.5 , &buffer[0], m_Font);
	}
	else
	{
		if ( Vertical_Speed_FPM > 19.0 )
		{
			sprintf(buffer, " %03d", (int) Vertical_Speed_FPM);
			globals->m_FontManager->Print( 1.0, 103.5 , &buffer[0], m_Font);
		}
	}

	globals->m_FontManager->Print( 2.0, VSpeedToNeedle(600.0),"6-",m_Font);
	globals->m_FontManager->Print( 2.0, VSpeedToNeedle(300.0),"3-",m_Font);
	globals->m_FontManager->Print( 2.0, VSpeedToNeedle(100.0),"1-",m_Font);
	globals->m_FontManager->Print( 2.0, VSpeedToNeedle(-100.0),"1-",m_Font);
	globals->m_FontManager->Print( 2.0, VSpeedToNeedle(-300.0),"3-",m_Font);
	globals->m_FontManager->Print( 2.0, VSpeedToNeedle(-600.0),"6-",m_Font);

	glLineWidth(3.5);
	glBegin(GL_LINES);

	// Horizontal center detent
	glVertex2f(5.0, VSpeedToNeedle(0.0) + 1.75);
	glVertex2f(10.0, VSpeedToNeedle(0.0) + 1.75);

	// Next draw the angled line that indicates climb rate
	// We need to add 1.75 to the calculated needle position because of how
	// GL interprets the line width
	glVertex2f( 5.0, VSpeedToNeedle(Vertical_Speed_FPM) + 1.75);
	glVertex2f( 30.0, m_NeedleCenter + 1.75);
	glEnd();

	glPopMatrix();
}

double VSI::VSpeedToNeedle(double vspd)
{
	double needle;

	if(vspd>=0)
	{
		if(vspd>600)
			vspd = 600;

		needle = (1-exp(-3*vspd/600)) * m_MaxNeedleDeflection + m_NeedleCenter;
	}
	else
	{
		vspd = fabs(vspd);

		if(vspd>600)
			vspd = 600;

		needle = m_NeedleCenter - (1-exp(-3*vspd/600)) * m_MaxNeedleDeflection;
	}

	return needle;
}

} // end namespace OpenGC

