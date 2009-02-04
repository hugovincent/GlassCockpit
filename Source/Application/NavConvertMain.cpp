#include "BinaryNavData.h"
#include "Debug.h"

int main(int argc, char **argv)
{
	OpenGC::BinaryNavData ndtb;

	LogPrintf("Converting airport data... ");
	ndtb.ConvertAirportData("/home/hugo/Projects/OpenGC/Data/Navigation/apt.dat", 
			"/home/hugo/Projects/OpenGC/Data/Navigation/apt_dat.bin");

	LogPrintf("Converting navaid data... ");
	ndtb.ConvertNavaidData("/home/hugo/Projects/OpenGC/Data/Navigation/nav.dat", 
			"/home/hugo/Projects/OpenGC/Data/Navigation/nav_dat.bin");

	LogPrintf("Done\n");
	
	return 0;
}

