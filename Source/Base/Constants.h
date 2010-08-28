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

/**
 * Global configuration definitions for OpenGC for Albatross as well as 
 * a configuration manager class.
 */

#ifndef Constants_h
#define Constants_h

#include <math.h>

namespace OpenGC {

// Numerical Constants and global Configuration Constants
const double DEG_TO_RAD = (M_PI / 180.0);	// degrees to radians multiplier
const double RAD_TO_DEG = (180.0 / M_PI);	// radians to degrees multiplier
const double METER_PER_NM = 1852.0;			// meters per nautical mile
const double MS_TO_KNOTS  = 1.94384449;		// meters per second to knots
const double METERS_TO_FEET = 3.2808399;	// meters to feet
const double EARTH_RADIUS  = 6371000.0;		// approx. radius of the earth in meters

// OpenGL Colours
#define COLOUR_YELLOW() glColor3ub(0,0,0) // FIXME

} // end namespace OpenGC

#endif
