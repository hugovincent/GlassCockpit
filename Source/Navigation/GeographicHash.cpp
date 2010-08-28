/*=========================================================================

  Copyright (c) 2001-2004 Damion Shelton
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

#include "GeographicHash.h"
#include <math.h>

namespace OpenGC
{

/* Longitude:  -180------------0------------180
 * Bin:           0           90            180
 *
 * Lattitude: Bin:
 *    90       90
 *    |
 *    |
 *    0        45
 *    |
 *    |
 *   -90       0
 */
GeographicHash::GeoListType& GeographicHash::GetListAtLatLon(double lat, double lon)
{
	int latBin = (int)floor(45 + lat/2);
	int lonBin = (int)floor(90 + lon/2);
	return m_GeoHash[latBin][lonBin];
}

void GeographicHash::InsertGeographicObject(GeographicObject* pObj)
{  
	GetListAtLatLon(pObj->GetDegreeLat(), pObj->GetDegreeLon()).push_back(pObj);
}

void GeographicHash::InsertGeographicList(GeographicObjectList* pList)
{
	for (GeoListType::iterator it = pList->begin(); it != pList->end(); ++it)
		this->InsertGeographicObject(*it);
}

} // end namespace OpenGC

