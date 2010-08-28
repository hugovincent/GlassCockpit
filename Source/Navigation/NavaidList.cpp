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
#include "NavaidList.h"
#include "NavaidGeoObj.h"
#include "Constants.h"
#include "BinaryNavData.h"
#include "Debug.h"

namespace OpenGC
{

using namespace std;

NavaidList::NavaidList()
{

}

NavaidList::~NavaidList()
{

}

bool NavaidList::LoadData(const string& fileName)
{
	ifstream file(fileName.c_str(), ios::binary);
	Assert(file.is_open(), "can't read navaid database");

	// The struct that is read in for each navaid
	BinaryNavData::NavaidData *nav = new BinaryNavData::NavaidData;

	// Now iterate over the structs in the file
	while (file.eof() != 1)
	{
		file.read((char*)nav, sizeof(BinaryNavData::NavaidData));

		// Ensure ID string is null terminated
		if (nav->id_length >= MAX_NAV_ID_LENGTH)
		{
			nav->id[MAX_NAV_ID_LENGTH] = 0;
		}
		else
		{
			nav->id[nav->id_length] = 0;
		}
		
		// Create the NavaidGeoObj and fill out it's fields
		NavaidGeoObj* pNavaid = new NavaidGeoObj();
		pNavaid->SetAltitudeMeters(nav->elev);
		pNavaid->SetIdentification(nav->id);
		pNavaid->SetDegreeLat(nav->lat);
		pNavaid->SetDegreeLon(nav->lon);
		// FIXME other info

		// Now add the navaid to the list
		this->push_back(pNavaid);
	}
	delete nav;
	return true;
}

} // end namespace OpenGC

