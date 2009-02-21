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

#include <math.h>

#include "Globals.h"
#include "MarkedDial.h"
#include "CircleEvaluator.h"
#include "Constants.h"

#define CALL_MEMBER_FN(object,ptrToMember)  ((object)->*(ptrToMember))

namespace OpenGC
{

MarkedDial::MarkedDial()
{
	m_Font = globals->m_FontManager->LoadDefaultFont();

	m_PhysicalPosition.x = 0;
	m_PhysicalPosition.y = 0;

	m_PhysicalSize.x = 30;
	m_PhysicalSize.y = 30;

	m_Scale.x = 1.0;
	m_Scale.y = 1.0;

	m_Min = 0.0, m_Max = 0.0;
	m_TickSpacing = 1.0, m_TickDivisor = 1.0;
	m_DataFn = 0;
}

MarkedDial::~MarkedDial()
{

}

void MarkedDial::Render()
{
	GaugeComponent::Render();

	double value = CALL_MEMBER_FN(globals->m_DataSource->GetAirframe(), m_DataFn)();

	if (value < m_Min)
		value = m_Min;
	else if (value > m_Max)
		value = m_Max;

	double R = 11.0;
	double minDegrees = 220.0;
	double maxDegrees = 100.0;

	double maxDegreesUse360 = maxDegrees;
	if (maxDegrees < minDegrees)
		maxDegreesUse360 += 360;

	double negativeoffset = 0;
	if (m_Min < 0)
		negativeoffset = m_Min*-1.0;

	double radians;
	char buf[10];
	//GLUquadric *qobj;

	glLineWidth(2.0);

	glTranslatef(18, 18, 0);

	// gauge
	glColor3ub(51, 51, 76);
	CircleEvaluator *aCircle = globals->m_CircleEvaluator;
	aCircle->SetDegreesPerPoint(10.0);
	aCircle->SetRadius(R);
	aCircle->SetOrigin(0.0, 0.0);
	aCircle->SetArcStartEnd(minDegrees, value / m_Max * (maxDegreesUse360 - minDegrees) + minDegrees);

	aCircle->ResetVertices();
	aCircle->AddVertex(0,0);
	aCircle->Evaluate();
	aCircle->Render(GL_TRIANGLE_FAN);

	/*qobj = gluNewQuadric();
	  gluPartialDisk(qobj, 0, R+1, 50, 1, minDegrees, value / m_Max * (maxDegreesUse360-minDegrees));
	  gluDeleteQuadric(qobj);*/

	double percentage = value / (m_Max-m_Min) ;
	double degree = minDegrees + ((maxDegreesUse360 - minDegrees) * percentage);
	radians = degree * DEG_TO_RAD;
	glColor3ub(255, 255, 255);
	const float vertices[] = {0,0,   R*sin(radians),R*cos(radians)};
	glVertexPointer(2, GL_FLOAT, 0, &vertices);
	glDrawArrays(GL_LINE_STRIP, 0, 2);

	//circle outside
	glColor3ub(255, 255, 255);
	aCircle->SetArcStartEnd(minDegrees, maxDegrees);

	// FIXME enable mitering
	aCircle->ResetVertices();
	aCircle->Evaluate();
	aCircle->Render(GL_LINE_STRIP);

	/*qobj = gluNewQuadric();
	  gluPartialDisk(qobj, R, R+1, 50, 1, minDegrees, maxDegreesUse360-minDegrees);
	  gluDeleteQuadric(qobj);*/

	// unit markers
	globals->m_FontManager->SetSize(m_Font, 4.0, 3.5);
	double percentagev, degreev;
	for(double xs = m_Min; xs <= m_Max; xs += m_TickSpacing)
	{
		percentagev = (xs+negativeoffset) / (m_Max-m_Min) ;
		degreev =  minDegrees+ ((maxDegreesUse360- minDegrees)*percentagev);
		radians=degreev * DEG_TO_RAD;
		glColor3ub(255, 255, 255);
		const float vertices[] = {R*sin(radians),R*cos(radians),   (R-2)*sin(radians),(R-2)*cos(radians)};
		glVertexPointer(2, GL_FLOAT, 0, &vertices);
		glDrawArrays(GL_LINE_STRIP, 0, 2);
		
		if ((fabs(xs) < 1.0) && (fabs(xs) > 0.01))
			sprintf(buf, "%0.1f",xs);
		else
			sprintf(buf, "%.0f",xs / m_TickDivisor);
		glTranslatef(-1.5, -2, 0);
		globals->m_FontManager->Print((R-4.5) * sin(radians) , (R-4.5) * cos(radians), buf, m_Font);			
		glTranslatef(1.5, 2, 0);
	}


	glPopMatrix();

}

} // end namespace OpenGC
