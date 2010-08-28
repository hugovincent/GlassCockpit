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
