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

#include <cmath>
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

