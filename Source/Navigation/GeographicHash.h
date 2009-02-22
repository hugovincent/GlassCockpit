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

#ifndef GeographicHash_h
#define GeographicHash_h

#include "GeographicObject.h"
#include "GeographicObjectList.h"
#include <list>
#include <vector>

#define NUM_BINS_LAT	90
#define NUM_BINS_LONG	180

namespace OpenGC
{

/* GeographicHash stores geographic objects in 2x2 degree bins for
 * efficient rendering access. */
class GeographicHash  
{
	public:

		GeographicHash() {}
		virtual ~GeographicHash() {}

		/** Type of linked list used to store all geo objects in each degree block */
		typedef std::list<GeographicObject*> GeoListType;

		/** Insert all of the members of a geographics list into the hash */
		void InsertGeographicList(GeographicObjectList* pList);

		/** Get the object list for a bin containing lat/lon */
		GeoListType& GetListAtLatLon(double lat, double lon);

	protected:

		/** Add a geographic object to the hash - for internal use */
		void InsertGeographicObject(GeographicObject* pObj);

		/** The spatial hash of geographic objects */
		GeoListType m_GeoHash[NUM_BINS_LAT][NUM_BINS_LONG];
};

} // end namespace OpenGC

#endif
