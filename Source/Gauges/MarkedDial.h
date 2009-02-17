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

#ifndef MarkedDial_H
#define MarkedDial_H

#include "GaugeComponent.h"

namespace OpenGC
{

typedef double (AirframeDataContainer::*AirframeMemFn)();

class MarkedDial : public GaugeComponent  
{
	public:

		MarkedDial();
		virtual ~MarkedDial();

		void Render();
		void SetDataSource(AirframeMemFn fn) {m_DataFn = fn; }
		void SetMinMax(double min, double max) {m_Min = min; m_Max = max; }
		void SetTickSpacing(double spacing) {m_TickSpacing = spacing; }
		void SetTickDivisor(double divisor) {m_TickDivisor = divisor; }

	protected:

		int m_Font;
		AirframeMemFn m_DataFn;
		double m_Min, m_Max, m_TickSpacing, m_TickDivisor;
};

}

#endif
