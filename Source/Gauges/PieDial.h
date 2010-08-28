/*=========================================================================

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

#ifndef PieDial_H
#define PieDial_H

#include "MarkedDial.h"
#include "CircleEvaluator.h"

namespace OpenGC
{

typedef float (AirframeDataContainer::*AirframeMemFn)();

class PieDial : public MarkedDial
{
	public:

		PieDial();
		virtual ~PieDial();

		void Render();

		/** Set a function pointer to the Airframe->Get__() function */
		void SetDataSource(AirframeMemFn fn) {m_DataFn = fn; }

		/** Set the range of values displayed on this gauge */
		void SetMinMax(double min, double max) {m_Min = min; m_Max = max; }

		/** Set the thresholds which if the value exceeds, the colour will change */
		void SetColourRanges(double minYellow, double minRed) {m_MinYellow = minYellow; m_MinRed = minRed; }

	protected:

		/** Used from Render() to draw the dials arc.
		 * Separate so it can be overloaded to draw the arc differently, 
		 * e.g. for the Tachometer. Drawing is done in a coordinate system
		 * with the origin at the center of the dial. */
		virtual void RenderArc(CircleEvaluator *circ);

		/** Used from Render() to draw the dials ticks.
		 * See RenderArc() for details. */
		virtual void RenderTicks(CircleEvaluator *circ);

		int m_Font;
		AirframeMemFn m_DataFn;
		double m_Min, m_Max;
		double m_MinYellow, m_MinRed;
	
		/** Drawing options */
		static const double R = 16.0;
		static const double minDegrees = 90.0;
		static const double maxDegrees = 300.0;	
};

} // end namespace OpenGC

#endif
