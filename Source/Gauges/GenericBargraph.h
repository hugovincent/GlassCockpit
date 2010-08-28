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
