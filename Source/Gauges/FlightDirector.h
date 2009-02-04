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

/**
 * Flight director (overlaid over PFD)
 */

#ifndef FlightDirector_h
#define FlightDirector_h

#include "GaugeComponent.h"

namespace OpenGC
{

class FlightDirector : public GaugeComponent
{
	public:
		FlightDirector();
		virtual ~FlightDirector();

		/** Overloaded render function */
		void Render();

	protected:
		/** The font number provided to us by the font manager */
		int m_Font;
};

} // end namespace OpenGC

#endif
