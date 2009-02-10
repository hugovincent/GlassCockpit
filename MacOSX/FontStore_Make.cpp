/*
 *  FontStore_Make.cpp
 *  GlassCockpit
 *
 *  Created by Hugo Vincent on 9/02/09.
 *  Copyright 2009 Hugo Vincent. All rights reserved.
 *
 */

#include "Debug.h"

#define USE_FREETYPE2
#include "Font_FileStore.h"

using namespace std;

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		LogPrintf("Usage: FontStore_Make fontname.ttf (generates fontstore.glfont)");
		return 1;
	}
	
	string basename = argv[1];
	Font_FileStore *fileStore = Font_FileStore::CreateFromTTF(basename + ".ttf");
	//Assert(fileStore != NULL, "error create font filestore");
	//fileStore->SerializeToFile(basename + ".glfont");
	
	delete fileStore;
	return 0;
}
