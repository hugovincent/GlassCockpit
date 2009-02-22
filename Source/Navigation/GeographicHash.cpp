/*=========================================================================

  OpenGC - The Open Source Glass Cockpit Project
  Please see our web site at http://www.opengc.org

  Albatross UAV Project - http://www.albatross-uav.org

  Copyright (c) 2001-2003 Damion Shelton
  All rights reserved.
  See Copyright.txt or http://www.opengc.org/Copyright.htm for details.

  This software is distributed WITHOUT ANY WARRANTY; without even 
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
  PURPOSE.  See the above copyright notice for more information.

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

