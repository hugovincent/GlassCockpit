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

#include "Debug.h"

// Note: compile with USE_FREETYPE2 defined (gcc -DUSE_FREETYPE2 ...)

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
	Assert(fileStore != NULL, "error create font filestore");
	fileStore->SerializeToFile(basename + ".glfont");
	
	delete fileStore;
	return 0;
}
