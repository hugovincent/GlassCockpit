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
 * Defines an ordered pair structure templated over data type (int, double, etc.)
 * and associated mathematical operations. All members are public to allow easy
 * access
 */

#ifndef OrderedPair_h
#define OrderedPair_h

namespace OpenGC
{

template<typename TDataType>
class OrderedPair
{

	public:

		OrderedPair() {x=0; y=0;}
		OrderedPair(char *init);
		~OrderedPair() {};

		/** X coordinate */
		TDataType x;

		/** Y coordinate */
		TDataType y;
};

} // end namespace OpenGC

#endif
