// FIXME add header etc

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
	void ConvertAirportData(const std::string& inFileName, 
			const std::string& outFileName) const;

	/** Parses an ASCII navaid database (nav.dat) and writes to the 
	 * binary output file */
	void ConvertNavaidData(const std::string& inFileName, 
			const std::string& outFileName) const;

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

