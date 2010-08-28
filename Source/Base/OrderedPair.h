/*=========================================================================

  Copyright (c) 2001-2003 Damion Shelton 
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
