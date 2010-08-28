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

#ifndef BinaryNavData_h
#define BinaryNavData_h

#include <string>

#define MAX_APT_ID_LENGTH 8
#define MAX_NAV_ID_LENGTH 8

namespace OpenGC {

/** Utility class for converting ASCII navigation files into binary format.
 * The binary format is around 12 times smaller on disk, and loads into 
 * OpenGC much, much faster. 
 * 
 * See http://www.x-plane.org/home/robinp/FileDef.htm for more info
 */
class BinaryNavData
{
public:
	/** Parses an ASCII airport database (apt.dat) and writes to the 
	 * binary output file */
	static void ConvertAirportData(const std::string& inFileName, 
			const std::string& outFileName);

	/** Parses an ASCII navaid database (nav.dat) and writes to the 
	 * binary output file */
	static void ConvertNavaidData(const std::string& inFileName, 
			const std::string& outFileName);

	/** Struct representing each navaid in the binary file format */
	typedef struct
	{
		float lat, lon, elev, frequency;
		unsigned char navaidType;
		unsigned char id_length;
		char id[MAX_NAV_ID_LENGTH];
	} NavaidData;

	/** Struct representing each airport in the binary file format */
	typedef struct
	{
		float lat, lon, elev;
		unsigned char id_length;
		char id[MAX_APT_ID_LENGTH];
	} AirportData;
};

} // end namespace OpenGC

#endif

