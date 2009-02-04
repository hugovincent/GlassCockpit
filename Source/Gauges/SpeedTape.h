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

/**
 * Boeing 777 style speed tape
 */

#ifndef SpeedTape_h
#define SpeedTape_h

#include "GaugeComponent.h"

namespace OpenGC
{

class SpeedTape : public GaugeComponent  
{
	public:

		SpeedTape();
		virtual ~SpeedTape();

		/** Overloaded render function */
		void Render();

	protected:

		/** The font number provided to us by the font manager */
		int m_Font;

		double indent_x;  // defines region of speed tape
};

} // end namespace OpenGC

#endif
