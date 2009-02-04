/*=========================================================================

  OpenGC - The Open Source Glass Cockpit Project
  Please see our web site at http://www.opengc.org

  Albatross UAV Project - http://www.albatross-uav.org

  Copyright (c) 2001-2003 Hugo Vincent
  All rights reserved.
  See Copyright.txt or http://www.opengc.org/Copyright.htm for details.

  This software is distributed WITHOUT ANY WARRANTY; without even 
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
  PURPOSE.  See the above copyright notice for more information.

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

