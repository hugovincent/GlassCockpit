/*=========================================================================

  OpenGC - The Open Source Glass Cockpit Project
  Please see our web site at http://www.opengc.org

  Albatross UAV Project - http://www.albatross-uav.org

  Copyright (c) 2001-2003 Hugo Vincent
  All rights reserved.
  See Copyright.txt or http://www.opengc.org/Copyright.htm for details.

  This software is distributed WITHOUT ANY WARRANTY; without even 
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
  PURPOSE.  See the above copyright notice for more information.

  =========================================================================*/

#include "GeographicObject.h"
#include "Constants.h"

namespace OpenGC
{

GeographicObject::GeographicObject()
{
	m_AltitudeMeters=0.0;
	m_DegreeLat=0.0;
	m_DegreeLon=0.0;
}

GeographicObject::~GeographicObject()
{

}

void GeographicObject::LatLonToMercator(double lat, double lon, double &northing, double &easting)
{
	easting = EARTH_RADIUS * (lon * DEG_TO_RAD);
	northing = EARTH_RADIUS * log(tan(M_PI / 4.0 + (lat * DEG_TO_RAD) / 2.0));
}

} // end namespace OpenGC

