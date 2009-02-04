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

/**
 * GeographicHash stores geographic objects in 1x1 degree bins for
 * efficient rendering access. Bins are numbered as:
 *                    PM
 *     -180------------0------------180
 *  -90  0            180           360
 *   |  361...etc
 *   |
 *   EQ 0
 *   |
 *   |
 *   90
 */

#ifndef GeographicHash_h
#define GeographicHash_h

#include "GeographicObject.h"
#include "GeographicObjectList.h"
#include <list>
#include <vector>

namespace OpenGC
{

class GeographicHash  
{

	public:

		GeographicHash();
		virtual ~GeographicHash();

		/** Type of linked list used to store all geo objects in each degree block */
		typedef std::list<GeographicObject*> GeoListType;

		/** Type of vector used to store the hash */
		typedef std::vector<GeoListType*> GeoVectorType;

		/** Insert all of the members of a geographics list into the hash */
		void InsertGeographicList(GeographicObjectList* pList);

		/** Get the object list at a particular lat/lon */
		GeoListType* GetListAtLatLon(double lat, double lon);

	protected:

		/** Convert a lat/lon pair to an index into the hash */
		int LatLonToBin(double lat, double lon);

		/** Add a geographic object to the hash - for internal use */
		void InsertGeographicObject(GeographicObject* pObj);

		/** The spatial hash of geographic objects */
		GeoVectorType m_GeoHash;

};

} // end namespace OpenGC

#endif
