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

