/*=========================================================================

  OpenGC - The Open Source Glass Cockpit Project
  Please see our web site at http://www.opengc.org

  Albatross UAV Project - http://www.albatross-uav.org

  Copyright (c) 2006-2006 Hugo Vincent.
  All rights reserved.
  See Copyright.txt or http://www.opengc.org/Copyright.htm for details.

  This software is distributed WITHOUT ANY WARRANTY; without even 
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
  PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include <math.h>

#include "GLHeaders.h"
#include "Tachometer.h"
#include "CircleEvaluator.h"
#include "Constants.h"

#define CALL_MEMBER_FN(object,ptrToMember)  ((object)->*(ptrToMember))

namespace OpenGC
{

Tachometer::Tachometer()
{
	m_DataFn = &AirframeDataContainer::GetEngine_RPM;
	m_Min = 0.0, m_Max = 9000.0;
	m_MinYellow = 6000.0, m_MinRed = 8000.0;
}

Tachometer::~Tachometer()
{

}

void Tachometer::Render()
{
	PieDial::Render();
}

// These Render___() Functions are overloaded to change the way the dial appears

void Tachometer::RenderTicks(CircleEvaluator *circ)
{
	// yellow stripe
	double percentagey = m_MinYellow / (m_Max - m_Min) ;
	double degreeyellow = minDegrees + ((maxDegrees - minDegrees) * percentagey);
	glColor3ub(247, 231, 8);
	double radians = degreeyellow * DEG_TO_RAD;
	glBegin(GL_LINE_STRIP);
	glVertex2f(R * sin(radians), R * cos(radians));
	glVertex2f((R + 4) * sin(radians), (R + 4) * cos(radians));
	glEnd();

	// red stripe
	double percentager = m_MinRed / (m_Max - m_Min) ;
	double degreered =  minDegrees + ((maxDegrees - minDegrees) * percentager);
	radians = degreered * DEG_TO_RAD;
	glColor3ub(255, 0, 0);
	glBegin(GL_LINE_STRIP);
	glVertex2f(R * sin(radians), R * cos(radians));
	glVertex2f((R + 4) * sin(radians), (R + 5) * cos(radians));
	glEnd();
}

void Tachometer::RenderArc(CircleEvaluator *circ)
{
	// white partial circle 
	glColor3ub(255, 255, 255);
	circ->SetArcStartEnd(minDegrees, maxDegrees);
	// FIXME enable mitering
	glLineWidth(3.0);
	glBegin(GL_LINE_STRIP);
	circ->Evaluate();
	glEnd();
}
} // end namespace OpenGC
