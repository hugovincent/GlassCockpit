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
