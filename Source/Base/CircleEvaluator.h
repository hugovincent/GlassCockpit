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

/**
 * CircleEvaluator generates a series of points which lie on the
 * perimeter of the circle. The circle is evaluated as an arc,
 * given a start and end point in degrees. Degrees are defined as 0=360 being
 * up, 90 degrees right, etc.
 */

#ifndef CircleEvaluator_h
#define CircleEvaluator_h

namespace OpenGC
{

class CircleEvaluator  
{

	public:

		CircleEvaluator();
		virtual ~CircleEvaluator();

		/** Sets the radius of the circle in physical units */
		void SetRadius(double radius);

		/** Defines the "coarseness" of the circle (degree spacing of GL_POINTS) */
		void SetDegreesPerPoint(double degreesPerPoint);

		/** Defines the start and end of the arc in degrees */
		void SetArcStartEnd(double startArc, double endArc);

		/** Defines the center of the circle in physical units */
		void SetOrigin(double x, double y);

		/** Add specified vertex to vertex array at the current position */
		void AddVertex(float x, float y);

		/** Evaluate the circle, generating points into vertex cache */
		void Evaluate();

		/** Renders vertex cache in specified mode */
		void Render(GLenum mode);

		/** Clears vertex cache */
		void ResetVertices();

	protected:

		/** The origin (center) of the circle */
		double m_XOrigin, m_YOrigin;

		/** The radius of the arc/circle */
		double m_Radius;

		/** The start and end of the arc in degrees */
		double m_StartArcDegrees, m_EndArcDegrees;

		/** How many degrees to move before generating a new point */
		double m_DegreesPerPoint;

		/** Used to hold GL generated vertices */
		float *m_Vertices;

		/** Current index into vertex array */
		unsigned int m_VertexIdx;
};

} // end namespace OpenGC

#endif
