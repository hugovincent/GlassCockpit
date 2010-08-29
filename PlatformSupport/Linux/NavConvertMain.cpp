#include "BinaryNavData.h"
#include "Debug.h"
#include <string>

using namespace std;

int main(int argc, char **argv)
{
	OpenGC::BinaryNavData ndtb;

	if (argc != 2)
	{
		printf("usage: navconvert Path/To/DataDir/Navigation\n");
		return 1;
	}

	LogPrintf("Converting airport data... ");
	ndtb.ConvertAirportData(string(argv[1]) + "apt.dat", 
			string(argv[1]) + "apt_dat.bin");

	LogPrintf("Converting navaid data... ");
	ndtb.ConvertNavaidData(string(argv[1]) + "nav.dat", 
			string(argv[1]) + "nav_dat.bin");

	LogPrintf("Done\n");
	
	return 0;
}

