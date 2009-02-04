/*=========================================================================

  OpenGC - The Open Source Glass Cockpit Project
  Please see our web site at http://www.opengc.org

  Albatross UAV Project - http://www.albatross-uav.org

  Copyright (c) 2006 Hugo Vincent
  All rights reserved.
  See Copyright.txt or http://www.opengc.org/Copyright.htm for details.

  This software is distributed WITHOUT ANY WARRANTY; without even 
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
  PURPOSE.  See the above copyright notice for more information.

  =========================================================================*/

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include "WaypointList.h"
#include "WaypointGeoObj.h"
#include "Constants.h"

namespace OpenGC
{

using namespace std;

WaypointList::WaypointList()
{

}

WaypointList::~WaypointList()
{

}

bool WaypointList::LoadData(const string& fileName)
{

	// Buffer used to read from file
	string lineData;

	// Open the input file for reading
	ifstream inputFile( fileName.c_str() );

	// Test to see if the file is open
	if( inputFile.is_open() == 0 )
	{
		printf("Error: unable to load the waypoint database file.\n");
		return false;
	}

	// Ok, file is open, process it
	while (inputFile.eof() != 1)
	{
		// Extract a line
		getline(inputFile, lineData);
		
		// Ignore blank lines
		if (lineData == "" || (lineData.size() == 1 && isspace(lineData[0]))) // FIXME need python string.strip() operation...
			continue;
		
		// Ignore comments (lines starting with "#")
		if (lineData[0] == '#')
			continue;

		// Variables to parse into
		double lat, lon, elev;
		int waypointStyle;
		string id;

		// Temp waypoint storage
		WaypointGeoObj* pWaypoint;

		// Input stream for parsing
		std::istringstream inputStream(lineData);

		// Parse and store the input line
		inputStream >> lat >> lon >> elev >> waypointStyle >> id;
		pWaypoint = new WaypointGeoObj();
		pWaypoint->SetDegreeLat(lat);
		pWaypoint->SetDegreeLon(lon);
		pWaypoint->SetAltitudeMeters(elev / METERS_TO_FEET);
		pWaypoint->SetStyle(waypointStyle);
		pWaypoint->SetIdentification(id);
		this->push_back(pWaypoint);
	}

	// Everything worked ok
	return true;
}

} // end namespace OpenGC

