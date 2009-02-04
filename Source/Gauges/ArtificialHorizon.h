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
 * Boeing 777 type artificial horizon
 */

#ifndef ArtificialHorizon_h
#define ArtificialHorizon_h

#include "GaugeComponent.h"

namespace OpenGC
{

class ArtificialHorizon : public GaugeComponent  
{
	public:

		ArtificialHorizon();
		virtual ~ArtificialHorizon();

		/** Overloaded render function */
		void Render();

	protected:

		/** The font number provided to us by the font manager */
		int m_Font;
};

} // end namespace OpenGC

#endif
