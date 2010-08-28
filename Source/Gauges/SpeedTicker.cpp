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

#include <stdio.h>

#include "Globals.h"
#include "GaugeComponent.h"
#include "SpeedTicker.h"

namespace OpenGC
{

SpeedTicker::SpeedTicker()
{
	m_Font = globals->m_FontManager->LoadDefaultFont();

	m_PhysicalPosition.x = 0;
	m_PhysicalPosition.y = 0;

	m_PhysicalSize.x = 21;
	m_PhysicalSize.y = 18;

	m_Scale.x = 1.0;
	m_Scale.y = 1.0;
}

SpeedTicker::~SpeedTicker()
{

}

void SpeedTicker::Render()
{
	// Call base class to setup viewport and projection
	GaugeComponent::Render();

	// Draw black background
	glColor3ub(0,0,0);
	// Rectangular part
	static const float vertices[] = {0.0,0.0,   18.0,0.0,   18.0,18.0,   0.0,18.0};
	glVertexPointer(2, GL_FLOAT, 0, &vertices);
	glDrawArrays(GL_QUADS, 0, 4);
	// Triangular part
	static const float vertices1[] = {18.0,7.0,   21.0,9.0,   18.0,11.0};
	glVertexPointer(2, GL_FLOAT, 0, &vertices1);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// White border around background
	glColor3ub(255,255,255);
	glLineWidth(2);
	static const float vertices2[] = {18.0,0.0,   0.0,0.0,   0.0,18.0,   18.0,18.0};
	glVertexPointer(2, GL_FLOAT, 0, &vertices2);
	glDrawArrays(GL_LINE_STRIP, 0, 4);
	glLineWidth(1.5);
	static const float vertices3[] = {18.0,18.0, 18.0,11.0,   21.0,9.0,   18.0,7.0,   18.0,0.0};
	glVertexPointer(2, GL_FLOAT, 0, &vertices3);
	glDrawArrays(GL_LINE_STRIP, 0, 5);

	char buffer[4];
	double airspeed = globals->m_DataSource->GetAirframe()->GetAirspeed_KT();
	
	if (airspeed > 999.0)
		airspeed = 999.0; 
	
	// Speed for integer calculations
	long int spd = (long int) airspeed;

	// y position of the text (for easy changes)
	const double fontHeight = 7.0;
	double texty = m_PhysicalSize.y / 2 - fontHeight / 2;

	globals->m_FontManager->SetSize(m_Font, 6.0, fontHeight);

	// Draw text in white
	glColor3ub(255,255,255);

	if(airspeed>=100.0)
	{
		// 100's
		sprintf(buffer, "%li", spd/100);
		globals->m_FontManager->Print(2.0, texty, &buffer[0], m_Font);
		spd = spd-100*(int)(spd/100);
	}

	if(airspeed>10.0)
	{
		// 10's
		sprintf(buffer, "%li", spd/10);
		globals->m_FontManager->Print(6.5, texty, &buffer[0], m_Font);
		spd = spd-10*(int)(spd/10);
	}

	// 1's - the most complicated, since they scroll
	// Middle digit
	int three_one = spd;
	// Now figure out the digits above and below
	int five_one = (three_one+2)%10;
	int four_one = (three_one+1)%10;
	int two_one = (three_one - 1 + 10)%10;
	int one_one = (three_one - 2 + 10)%10;

	// Figure out the Speed translation factor for the one's place
	glTranslated(0, -1*(airspeed - (int)airspeed)*fontHeight, 0);

	// Display all of the digits
	sprintf(buffer, "%i", five_one);
	globals->m_FontManager->Print(11.0, texty+fontHeight*2+fontHeight/5, &buffer[0], m_Font);

	sprintf(buffer, "%i", four_one);
	globals->m_FontManager->Print(11.0, texty+fontHeight+fontHeight/10, &buffer[0], m_Font);

	sprintf(buffer, "%i", three_one);
	globals->m_FontManager->Print(11.0, texty, &buffer[0], m_Font);

	sprintf(buffer, "%i", two_one);
	globals->m_FontManager->Print(11.0, texty-fontHeight-fontHeight/10, &buffer[0], m_Font);

	sprintf(buffer, "%i", one_one);
	globals->m_FontManager->Print(11.0, texty-fontHeight*2-fontHeight/5, &buffer[0], m_Font);
}

} // end namespace OpenGC
