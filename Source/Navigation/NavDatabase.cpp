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

#include "NavDatabase.h"
#include "BinaryNavData.h"
#include "Debug.h"
#include "Globals.h"
#include <stdio.h>
#include <sys/stat.h> 

namespace OpenGC
{

NavDatabase::NavDatabase()
{
	m_NavaidList = 0, m_NavaidHash = 0;
	m_AirportList = 0, m_AirportHash = 0;
	m_WaypointList = 0;
	m_FlightCourse = new FlightCourse();
}

NavDatabase::~NavDatabase()
{
	if(m_NavaidList != 0)
		delete m_NavaidList;

	if(m_NavaidHash != 0)
		delete m_NavaidHash;

	if(m_AirportList != 0)
		delete m_AirportList;
	
	if(m_AirportHash != 0)
		delete m_AirportHash;

	if(m_WaypointList != 0)
		delete m_WaypointList;

	if(m_FlightCourse != 0)
		delete m_FlightCourse;
}
	
bool fileExists(const string& filename)
{
	struct stat stFileInfo;
	if(stat(filename.c_str(), &stFileInfo) == 0) 
		return true;
	else
		return false;
}

void NavDatabase::InitDatabase()
{	
	const string& pathToNav = globals->m_PrefManager->GetPrefS("PathToData");
	const string& pathToWritableNav = globals->m_PrefManager->GetPrefS("PathToCaches");;
	
	// Check for cached binary copies of nav data otherwise convert and cache
	if (!fileExists(pathToWritableNav + "nav_dat.bin"))
	{
		LogPrintf("Generating binary cache of navaid data...\n");
		BinaryNavData::ConvertNavaidData(pathToNav + "Navigation/nav.dat", pathToWritableNav + "nav_dat.bin");
	}
	if (!fileExists(pathToWritableNav + "apt_dat.bin"))
	{
		LogPrintf("Generating binary cache of airport data...\n");
		BinaryNavData::ConvertAirportData(pathToNav + "Navigation/apt.dat", pathToWritableNav + "apt_dat.bin");
	}
	
	printf("NavDatabase: Loading navigation database... "); fflush(stdout);

	// Load and hash NavAid (binary) database
	m_NavaidList = new NavaidList;
	m_NavaidList->InitializeList(pathToWritableNav + "nav_dat.bin");
	m_NavaidHash = new GeographicHash;
	m_NavaidHash->InsertGeographicList(m_NavaidList);

	// Load and hash Airport (binary) database
	m_AirportList = new AirportList;
	m_AirportList->InitializeList(pathToWritableNav + "apt_dat.bin");
	m_AirportHash = new GeographicHash;
	m_AirportHash->InsertGeographicList(m_AirportList);

	// Load Waypoint database
	m_WaypointList = new WaypointList;
	m_WaypointList->InitializeList(pathToNav + "Navigation/waypoint.dat");

	// Load and hash Map database
	// FIXME actually load the map shapes database

	printf("Done\nNavDatabase: Navigation database contains:\n");
	printf("\t%i NavAids\n\t%i Airports\n\t%i Waypoints\n\t%i Map shapes\n",
			(int)m_NavaidList->size(), (int)m_AirportList->size(), 
			(int)m_WaypointList->size(), 0 /* m_MapShapeList->size() */);
}

} // end namespace OpenGC

