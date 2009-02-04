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

GeographicHash::GeographicHash()
{
	// Create new lists for every 1 degree lat/lon block
	GeoListType* pTempList;

	for (int i = 0; i < 64800; i++)
	{
		// Note that the type of _internal_ list is _not_ an GeographicObjectList
		// This is because the list object is designed to explicitly manage the storage
		// of list entries, while the list stored in the hash just holds a bunch of
		// pointers (like a normal linked list)
		pTempList = new GeoListType;
		m_GeoHash.push_back(pTempList);
	}
}

GeographicHash::~GeographicHash()
{
	GeoVectorType::iterator it;

	// Delete all of the lists
	for (it = m_GeoHash.begin(); it != m_GeoHash.end(); ++it)
	{
		if ( (*it)!=0)
			delete (*it);
	}
}

void GeographicHash::InsertGeographicList(GeographicObjectList* pList)
{
	GeoListType::iterator it;

	// Loop through all geographic objects and put them into the hash
	for (it = pList->begin(); it != pList->end(); ++it)
	{
		this->InsertGeographicObject(*it);
	}
}

int GeographicHash::LatLonToBin(double lat, double lon)
{
	int lonBin = (int)floor(lon) + 180;
	int latBin = (int)floor(lat) + 90;

	int index = latBin * 360 + lonBin;

	return index;
}

GeographicHash::GeoListType* GeographicHash::GetListAtLatLon(double lat, double lon)
{
	return m_GeoHash[ this->LatLonToBin(lat,lon) ];
}

void GeographicHash::InsertGeographicObject(GeographicObject* pObj)
{  
	// Put the geographic object into the appropriate list
	m_GeoHash[ this->LatLonToBin(pObj->GetDegreeLat(), pObj->GetDegreeLon()) ]->push_back(pObj);
}

} // end namespace OpenGC

