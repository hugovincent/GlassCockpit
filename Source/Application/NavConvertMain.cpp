#include "BinaryNavData.h"
#include "Debug.h"

int main(int argc, char **argv)
{
	OpenGC::BinaryNavData ndtb;

	LogPrintf("Converting airport data... ");
	ndtb.ConvertAirportData("/Users/hugo/Projects/iPhone/GlassCockpit/Data/Navigation/apt.dat", 
			"/Users/hugo/Projects/iPhone/GlassCockpit/Data/Navigation/apt_dat.bin");

	LogPrintf("Converting navaid data... ");
	ndtb.ConvertNavaidData("/Users/hugo/Projects/iPhone/GlassCockpit/Data/Navigation/nav.dat", 
			"/Users/hugo/Projects/iPhone/GlassCockpit/Data/Navigation/nav_dat.bin");

	LogPrintf("Done\n");
	
	return 0;
}

