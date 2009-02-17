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

#ifndef Tachometer_H
#define Tachometer_H

#include "PieDial.h"

namespace OpenGC
{

class Tachometer : public PieDial
{
	public:
		Tachometer();
		virtual ~Tachometer();

		void Render();

	protected:
		virtual void RenderArc(CircleEvaluator *circ);
		virtual void RenderTicks(CircleEvaluator *circ);
		
		double m_MaxYellow, m_MaxRed;
};

} // end namespace OpenGC

#endif
