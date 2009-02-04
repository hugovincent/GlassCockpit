/*=========================================================================

  OpenGC - The Open Source Glass Cockpit Project
  Please see our web site at http://www.opengc.org

  Albatross UAV Project - http://www.albatross-uav.org

  Copyright (c) 2006 Hugo Vincent
  All rights reserved.
  See Copyright.txt or http://www.opengc.org/Copyright.htm for details.

  This software is distributed WITHOUT ANY WARRANTY; without even 
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
  PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#ifndef EngineDial_H
#define EngineDial_H

#include "GenericDial.h"
#include "CircleEvaluator.h"

namespace OpenGC
{

typedef double (AirframeDataContainer::*AirframeMemFn)();

class EngineDial : public GenericDial
{
	public:

		EngineDial();
		virtual ~EngineDial();

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

		// Drawing options
		static const double R = 16.0;
		static const double minDegrees = 90.0;
		static const double maxDegrees = 300.0;
};

} // end namespace OpenGC

#endif
