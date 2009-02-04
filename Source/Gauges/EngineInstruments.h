/*=========================================================================

  OpenGC - The Open Source Glass Cockpit Project
  Please see our web site at http://www.opengc.org

  Albatross UAV Project - http://www.albatross-uav.org

  Copyright (c) 2001-2003 Hugo Vincent.
  All rights reserved.
  See Copyright.txt or http://www.opengc.org/Copyright.htm for details.

  This software is distributed WITHOUT ANY WARRANTY; without even 
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
  PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

/**
 * Albatross engine instruments.
 */

#ifndef EngineInstruments_h
#define EngineInstruments_h

#include "Gauge.h"

namespace OpenGC
{

class EngineInstruments : public Gauge
{
	public:
		EngineInstruments();
		virtual ~EngineInstruments();

		/** Overloaded render function */
		void Render();

	protected:
		/** The font number provided to us by the font manager */
		int m_Font;

		/** Calculates frames per second. */
		double GetFPS();
};

} // end namespace OpenGC

#endif
