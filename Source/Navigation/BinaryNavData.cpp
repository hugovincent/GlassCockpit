// FIXME add header etc

#include <iostream>
#include <sstream>
#include <fstream>

#include "Constants.h"
#include "Debug.h"
#include "BinaryNavData.h"

using namespace std;

namespace OpenGC {

/** Tested with X-Plane APT data version 850, release 2009.1 */
void BinaryNavData::ConvertAirportData(const string& inFileName, 
		const string& outFileName)
{
	string lineData; // Buffer used to read lines from input file
	AirportData *apt = new AirportData; // Output struct
	int outSize = 0, numAirports = 0; // Counts for stats printed at the end

	// Open the input file for reading
	ifstream inputFile(inFileName.c_str());
	Assert(inputFile.is_open(), "can't read airport database");

	// Open the binary output file for writing
	FILE* outputFile = fopen(outFileName.c_str(), "w");
	Assert(outputFile != NULL, "can't open output file");

	// Throw out the first two header lines
	getline(inputFile, lineData);
	getline(inputFile, lineData);
	LogPrintf("Airport Data Version: %s\n", lineData.c_str());

	while (inputFile.eof() != 1)
	{
		int airportHeader; // Should equal 1 if we have an airport
		bool foundAnAirport = false; // Used to loop until we find an airport
		while (foundAnAirport == false)
		{
			// Read the line
			getline(inputFile, lineData);
			
			// Skip blank lines
			if (lineData.length() == 1 && (lineData[0] == '\r' || lineData[0] == '\n'))
				continue;

			// Airports are coded as 1
			sscanf(lineData.c_str(), "%d", &airportHeader);
			if (airportHeader == 1)
			{
				foundAnAirport = true;
			}

			// Check to see if we're at the last line in the file
			if (airportHeader == 99)
			{
				break;
			}
		}

		// If we're here without finding an airport, we've reached the EOF
		if (foundAnAirport == false)
			continue;

		// Potential data that can be loaded
		double lat, lon, elev;
		bool hasControlTower, displayDefaultBuildings;
		string id;
		
		// Parse the line
		istringstream inputStream(lineData);
		inputStream >> airportHeader >> elev >> hasControlTower >> 
			displayDefaultBuildings >> id;
		Assert(airportHeader == 1, "unexpected file format");
		
		// Search for a runway/helipad/etc
		do
		{
			getline(inputFile, lineData);
			sscanf(lineData.c_str(), "%d", &airportHeader);
		}
		while (airportHeader != 10 && airportHeader != 100 && airportHeader != 101 && airportHeader != 102);
			
		if (airportHeader == 10) // 810 version
		{
			// This is one end of the first runway, which is not a great 
			// approximation of the actual airport location, but it's simple.
			inputStream.str(lineData);
			inputStream >> airportHeader >> lat >> lon;
		}
		else if (airportHeader == 100) // 850 version runway
		{
			inputStream.str(lineData);
			// Info we have to parse before we get to lat/long
			float runwayLength, smoothness;
			int surfaceCode, shoulderCode, centerLights, edgeLights, distanceSigns;
			string runwayNumber;
			inputStream >> airportHeader >> runwayLength >> surfaceCode >>
				shoulderCode >> smoothness >> centerLights >> edgeLights >>
				distanceSigns >> runwayNumber >> lat >> lon;
		}
		else if (airportHeader == 101) // 850 version seaplane water runway
		{
			printf("Ignoring v850 seaplane water runway\n");
			continue; // FIXME
		}
		else if (airportHeader == 102) // 850 version helipad
		{
			printf("Ignoring v850 helipad\n");
			continue; // FIXME
		}
		
		// Store the data in the struct
		apt->elev = elev / OpenGC::METERS_TO_FEET;
		apt->id_length = id.size();
		strncpy(apt->id, id.c_str(), MAX_APT_ID_LENGTH);
		apt->lat = lat;
		apt->lon = lon;

		// Now write structure to file
		fwrite(apt, sizeof(AirportData), 1, outputFile);
		outSize += sizeof(AirportData);
		numAirports += 1;
	}

	LogPrintf("%d airports read, %d bytes written.\n", numAirports, outSize);

	inputFile.close();
	fclose(outputFile);
	delete apt;
}

/** Tested with X-Plane NAV data version 810, release 2009.1 */
void BinaryNavData::ConvertNavaidData(const string& inFileName, 
		const string& outFileName)
{
	string lineData; // Buffer used to read lines from input file
	NavaidData *nav = new NavaidData; // Output struct
	int outSize = 0, numNavaids = 0; // Counts for stats printed at the end

	// Open the input file for reading
	ifstream inputFile(inFileName.c_str());
	Assert(inputFile.is_open(), "can't read navaid database");

	// Open the binary output file for writing
	FILE* outputFile = fopen(outFileName.c_str(), "w");
	Assert(outputFile != NULL, "can't open output file");

	// Throw out the first two header lines
	getline(inputFile, lineData);
	getline(inputFile, lineData);
	LogPrintf("Nav Data Version: %s\n", lineData.c_str());

	while (inputFile.eof() != 1)
	{
		// Read the line
		getline(inputFile, lineData);
		
		// Skip blank lines
		if (lineData.length() == 1 && (lineData[0] == '\r' || lineData[0] == '\n'))
			continue;

		// Find what type of navaid it is
		int navaidType;
		sscanf(lineData.c_str(), "%d", &navaidType);

		// Check to see if we're at the last line in the file
		if(navaidType == 99)
		{
			break;
		}

		// Potential data that can be loaded
		float lat, lon, elev, freq, range, typeSpecific;
		string id;

		// Parse the line
		std::istringstream inputStream(lineData);
		inputStream >> navaidType >> lat >> lon >> elev >> freq >> 
			range >> typeSpecific >> id;

		// Navaid specific data handling:
		switch(navaidType)
		{
			case 2: // NDB
				nav->navaidType = 0; // <see NavaidGeoObj.h>
				break;
			case 3: // VOR, VORTAC, or VOR-DME
				freq = freq / 100.0;
				nav->navaidType = 1; // <see NavaidGeoObj.h>
				break;
			case 12:
			case 13: // DME component of VOR, VORTAC, or VOR-DME
				freq = freq / 100.0;
				nav->navaidType = 2; // <see NavaidGeoObj.h>
				break;
			case 4: // Localiser that is part of a full ILS
			case 5: // Standalone localizer (LLZ) including SDF (simplified directional facility) or LDF (landing directional facility)
			case 6: // GS (glide scope)
			case 7: // OM (outer marker)
			case 8: // MM (middle marker)
			case 9: // IM (inner marker)
				// FIXME we ignore all these types of navaids for now
				continue;
			default: // anything else
				LogPrintf("Unknown navaid type %d\n", navaidType);
				break;
		}

		// Store the data in the struct
		nav->lat = lat;
		nav->lon = lon;
		nav->elev = elev / OpenGC::METERS_TO_FEET;
		nav->frequency = freq;
		nav->id_length = id.size();
		strncpy(nav->id, id.c_str(), MAX_NAV_ID_LENGTH);

		// Now write structure to file
		fwrite(nav, sizeof(NavaidData), 1, outputFile);
		outSize += sizeof(NavaidData);
		numNavaids += 1;
	}

	LogPrintf("%d navaids read, %d kBytes written.\n", numNavaids, outSize / 1024);

	inputFile.close();
	fclose(outputFile);
	delete nav;
}

} // end namespace OpenGC

