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

#ifndef GeographicObjectList_h
#define GeographicObjectList_h

#include <list>
#include <string>
#include "GeographicObject.h"

namespace OpenGC
{

class GeographicObjectList : public std::list<GeographicObject*>
{
	public:

		GeographicObjectList();
		virtual ~GeographicObjectList();

		/** Initialize the object list - this consists of loading the data and 
		 * computing positions in coordinates other than lat / lon. The data
		 * loading stage should be overridden when implementing a derived 
		 * class. Will automatically call either LoadDataBin or LoadDataAscii
		 * depending on extension of filename (.bin->binary, .dat->ASCII */
		bool InitializeList(const string& filename);

		/** Read binary or ASCII file that contains data, returns true if
		 * successful,  else false. */
		virtual bool LoadData(const string& fileName) { return false; };

		/** Computes additional coordinate positions for the objects in the 
		 * list. This is called by InitializeList(). */
		void ComputeAdditionalCoordinates();
};

} // end namespace OpenGC

#endif

