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

