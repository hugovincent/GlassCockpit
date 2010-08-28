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
#include <math.h>
#include "GLHeaders.h"
#include "CircleEvaluator.h"

namespace OpenGC
{

CircleEvaluator::CircleEvaluator()
{
	m_XOrigin = 0;
	m_YOrigin = 0;

	m_StartArcDegrees = 0;
	m_EndArcDegrees = 0;

	m_DegreesPerPoint = 10;

	m_Radius = 1;
	
	m_Vertices = new float[2*1024]; // max 1024 vertices (each x,y)
	m_VertexIdx = 0;
}

CircleEvaluator::~CircleEvaluator()
{
	delete[] m_Vertices;
}

void CircleEvaluator::SetOrigin(double x, double y)
{
	m_XOrigin = x;
	m_YOrigin = y;
}

void CircleEvaluator::SetArcStartEnd(double startArc, double endArc)
{
	m_StartArcDegrees = startArc;
	m_EndArcDegrees = endArc;
}

void CircleEvaluator::SetDegreesPerPoint(double degreesPerPoint)
{
	m_DegreesPerPoint = degreesPerPoint;
}

void CircleEvaluator::AddVertex(float x, float y)
{
	m_Vertices[2*m_VertexIdx]   = x;
	m_Vertices[2*m_VertexIdx+1] = y;
	m_VertexIdx++;
}

void CircleEvaluator::Render(GLenum mode)
{
	glVertexPointer(2, GL_FLOAT, 0, m_Vertices);
	glDrawArrays(mode, 0, m_VertexIdx);
}

void CircleEvaluator::ResetVertices()
{
	m_VertexIdx = 0;
}

void CircleEvaluator::Evaluate()
{
	// Add the vertexes specified
	double x;
	double y;

	double pi = 3.14159265;

	// We parametrically evaluate the circle
	// x = sin(t)
	// y = cos(t)
	// t goes from 0 to 2pi
	// 0 degrees = 0rad, 90 degrees = pi/2rad, etc.

	double startRad = m_StartArcDegrees / 180 * pi;
	double endRad = m_EndArcDegrees / 180 * pi;
	double radPerPoint = m_DegreesPerPoint / 180 * pi;

	if (startRad > endRad)
		endRad += 2*pi;

	double currentRad = startRad;

	do
	{
		x = m_Radius*sin(currentRad) + m_XOrigin;
		y = m_Radius*cos(currentRad) + m_YOrigin;

		AddVertex(x,y);

		currentRad += radPerPoint;

	} while(currentRad < endRad);

	x = m_Radius*sin(endRad) + m_XOrigin;
	y = m_Radius*cos(endRad) + m_YOrigin;

	AddVertex(x,y);
}

void CircleEvaluator::SetRadius(double radius)
{
	if (radius > 0)
		m_Radius = radius;
	else
		m_Radius = 1;
}

} // end namespace OpenGC
