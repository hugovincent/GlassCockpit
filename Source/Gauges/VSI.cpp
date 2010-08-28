/*=========================================================================

  Copyright (c) 2001-2004 Damion Shelton
  Copyright (c) 2005-2010 Hugo Vincent <hugo.vincent@gmail.com>
  All rights reserved.
  
  This project is distributed under the terms of the GNU General Public License
  Version 3 <http://www.gnu.org/licenses/gpl.html>.
  
      This program is free software: you can redistribute it and/or modify
      it under the terms of the GNU General Public License as published by
      the Free Software Foundation, specifically version 3 of the License.
  
      This program is distributed in the hope that it will be useful,
      but WITHOUT ANY WARRANTY; without even the implied warranty of
      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
      GNU General Public License for more details.
  
      You should have received a copy of the GNU General Public License
      along with this program.  If not, see <http://www.gnu.org/licenses/>.

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
	static const float vertices[] = {0.0,10.0,   0.0,40.0,   4.5,44.0,   16.0,44.0,   16.0,27.0,   7.0,10.0};
	glVertexPointer(2, GL_FLOAT, 0, &vertices);
	glDrawArrays(GL_POLYGON, 0, 6);
	glVertexPointer(2, GL_FLOAT, 0, &vertices);
	glDrawArrays(GL_LINE_STRIP, 0, 6);

	// Center rectangle
	static const float vertices2[] = {4.5,44.0,   4.5,66.0,   16.0,66.0,   16.0,44.0};
	glVertexPointer(2, GL_FLOAT, 0, &vertices2);
	glDrawArrays(GL_POLYGON, 0, 4);
	glVertexPointer(2, GL_FLOAT, 0, &vertices2);
	glDrawArrays(GL_LINE_STRIP, 0, 4);

	// Top angular part
	static const float vertices3[] = {4.5,66.0,   0.0,70.0,   0.0,100.0,   7.0,100.0,   16.0,83.0,   16.0,66.0};
	glVertexPointer(2, GL_FLOAT, 0, &vertices3);
	glDrawArrays(GL_POLYGON, 0, 6);
	glVertexPointer(2, GL_FLOAT, 0, &vertices3);
	glDrawArrays(GL_LINE_STRIP, 0, 6);

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
	const float vertices4[] = {
		// Horizontal center detent
		5.0, VSpeedToNeedle(0.0) + 1.75,
		10.0, VSpeedToNeedle(0.0) + 1.75,

		// Next draw the angled line that indicates climb rate
		// We need to add 1.75 to the calculated needle position because of how
		// GL interprets the line width
		5.0, VSpeedToNeedle(Vertical_Speed_FPM) + 1.75,
		30.0, m_NeedleCenter + 1.75
	};
	glVertexPointer(2, GL_FLOAT, 0, &vertices4);
	glDrawArrays(GL_LINES, 0, 4);

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

