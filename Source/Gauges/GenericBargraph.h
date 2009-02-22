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

#ifndef GenericBargraph_H
#define GenericBargraph_H

#include "GaugeComponent.h"

namespace OpenGC
{

typedef float (AirframeDataContainer::*AirframeMemFn)();

class GenericBargraph : public GaugeComponent
{
	public:

		GenericBargraph();
		virtual ~GenericBargraph();

		void Render();

		/** Set a function pointer to the Airframe->Get__() function */
		void SetDataSource(AirframeMemFn fn) {m_DataFn = fn; }

		/** Set the range of values displayed on this gauge */
		void SetMinMax(double min, double max) {m_Min = min; m_Max = max; }

		/** Set the thresholds which if the value drops below the colour will change */
		void SetColourRanges(double maxYellow, double maxRed) {m_MaxYellow = maxYellow; m_MaxRed = maxRed; }

	protected:

		int m_Font;
		AirframeMemFn m_DataFn;
		double m_Min, m_Max;
		double m_MaxYellow, m_MaxRed;
};

} // end namespace OpenGC

#endif
