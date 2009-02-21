/*=========================================================================

  OpenGC - The Open Source Glass Cockpit Project
  Please see our web site at http://www.opengc.org

  Albatross UAV Project - http://www.albatross-uav.org

  Copyright (c) 2006 Hugo Vincent.
  All rights reserved.
  See Copyright.txt or http://www.opengc.org/Copyright.htm for details.

  This software is distributed WITHOUT ANY WARRANTY; without even 
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
  PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include <math.h>

#include "Globals.h"
#include "GLHeaders.h"
#include "PieDial.h"
#include "Constants.h"

#define CALL_MEMBER_FN(object,ptrToMember)  ((object)->*(ptrToMember))

namespace OpenGC
{
	
const double PieDial::R;
const double PieDial::minDegrees;
const double PieDial::maxDegrees;

PieDial::PieDial()
{
	m_Font = globals->m_FontManager->LoadDefaultFont();

	m_PhysicalPosition.x = 0;
	m_PhysicalPosition.y = 0;

	m_PhysicalSize.x = 42;
	m_PhysicalSize.y = 34;

	m_Scale.x = 1.0;
	m_Scale.y = 1.0;

	m_DataFn = 0;
	m_Min = 0.0, m_Max = 0.0;
}

PieDial::~PieDial()
{

}

void PieDial::Render()
{
	GaugeComponent::Render();

	double value = CALL_MEMBER_FN(globals->m_DataSource->GetAirframe(), m_DataFn)();

	if (value < m_Min)
		value = m_Min;
	if (value > m_Max)
		value = m_Max;

	char buf[10];
	//GLUquadric *qobj;

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glTranslatef(20, 20, 0);

	// gauge
	if (value < m_MinYellow)
		glColor3ub(51, 51, 76); // blue-grey
	else if (value < m_MinRed)
		glColor3ub(247, 231, 8); // yellow
	else
		glColor3ub(255, 20, 20); // red

	// Fill of the dial from 0 to the needle
	CircleEvaluator *aCircle = globals->m_CircleEvaluator;
	aCircle->SetDegreesPerPoint(10.0);
	aCircle->SetRadius(R);
	aCircle->SetOrigin(0.0, 0.0);
	aCircle->SetArcStartEnd(minDegrees, value / m_Max * (maxDegrees - minDegrees) + minDegrees);

	aCircle->ResetVertices();
	aCircle->AddVertex(0,0);
	aCircle->Evaluate();
	aCircle->Render(GL_TRIANGLE_FAN);

	// White line that is this needle of the dial
	double degree = minDegrees + ((maxDegrees - minDegrees) * (value / (m_Max-m_Min)));
	double radians = degree * DEG_TO_RAD;
	glColor3ub(255, 255, 255);
	glLineWidth(2.0);
	const float vertices[] = {0, 0, R * sin(radians), R * cos(radians)};
	glVertexPointer(2, GL_FLOAT, 0, &vertices);
	glDrawArrays(GL_LINE_STRIP, 0, 2);

	RenderTicks(aCircle);
	RenderArc(aCircle);
	glTranslatef(-20, -20, 0);

	// white rectangle containing the text
	glColor3ub(255, 255, 255);
	glLineWidth(1.0);
	static const float vertices2[] = {42,20,   20,20,   20,30,   42,30};
	glVertexPointer(2, GL_FLOAT, 0, &vertices2);
	glDrawArrays(GL_LINE_STRIP, 0, 4);

	// text
	globals->m_FontManager->SetSize(m_Font, 5, 5);
	glColor3ub(255, 255, 255);
	sprintf(buf, "%.0f", value);
	globals->m_FontManager->Print( 21.9, 22.7, buf, m_Font);

	glPopMatrix();
}

void PieDial::RenderTicks(CircleEvaluator *circ)
{
	// yellow stripe
	double percentagey = m_MinYellow / (m_Max - m_Min) ;
	double degreeyellow = minDegrees + ((maxDegrees - minDegrees) * percentagey);
	glColor3ub(247, 231, 8);
	double radians = degreeyellow * DEG_TO_RAD;
	const float vertices[] = {
		R * sin(radians), R * cos(radians),
		(R + 4) * sin(radians), (R + 4) * cos(radians)
	};
	glVertexPointer(2, GL_FLOAT, 0, &vertices);
	glDrawArrays(GL_LINE_STRIP, 0, 2);

	// red stripe
	double percentager = m_MinRed / (m_Max - m_Min) ;
	double degreered =  minDegrees + ((maxDegrees - minDegrees) * percentager);
	radians = degreered * DEG_TO_RAD;
	glColor3ub(255, 0, 0);
	const float vertices2[] = {
		R * sin(radians), R * cos(radians),
		(R + 4) * sin(radians), (R + 5) * cos(radians)
	};
	glVertexPointer(2, GL_FLOAT, 0, &vertices2);
	glDrawArrays(GL_LINE_STRIP, 0, 2);
}

void PieDial::RenderArc(CircleEvaluator *circ)
{
	// white partial circle 
	glColor3ub(255, 255, 255);
	circ->SetArcStartEnd(minDegrees, maxDegrees);
	// FIXME enable mitering
	glLineWidth(3.0);
	circ->ResetVertices();
	circ->Evaluate();
	circ->Render(GL_LINE_STRIP);
}

} // end namespace OpenGC
