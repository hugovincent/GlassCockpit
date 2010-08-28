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
 * NavDatabase is the container that stores all other nav information
 * used by OpenGC.
 */

#ifndef NavDatabase_h
#define NavDatabase_h

#include <string>
#include "NavaidList.h"
#include "AirportList.h"
#include "WaypointList.h"
#include "FlightCourse.h"
#include "GeographicHash.h"

namespace OpenGC
{

class NavDatabase  
{
	public:

		NavDatabase();
		virtual ~NavDatabase();

		/** Load the nav data */
		void InitDatabase();

		/** Get the hashed navaid list */
		GeographicHash* GetNavaidHash() {return m_NavaidHash;}

		/** Get the hashed airport list */
		GeographicHash* GetAirportHash() {return m_AirportHash;}

		/** Get the waypoint list */
		WaypointList* GetWaypointList() {return m_WaypointList;}

		/** Get the flight path */
		FlightCourse* GetFlightCourse() {return m_FlightCourse;}

	private:
		/** The flight path/course */
		FlightCourse *m_FlightCourse;
		
		/** A list of all waypoints */
		WaypointList *m_WaypointList;

		/** Airport list */
		AirportList *m_AirportList;

		/** Airport list hashed by lat/lon */
		GeographicHash *m_AirportHash;

		/** Navaid list */
		NavaidList *m_NavaidList;
		
		/** Navaid list hashed by lat/lon */
		GeographicHash *m_NavaidHash;
};

} // end namespace OpenGC

#endif

