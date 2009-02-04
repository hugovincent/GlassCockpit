/*=========================================================================

  OpenGC - The Open Source Glass Cockpit Project
  Please see our web site at http://www.opengc.org

  Albatross UAV Project - http://www.albatross-uav.org

  Copyright (c) 2001-2003 Hugo Vincent.
  All rights reserved.
  See Copyright.txt or http://www.opengc.org/Copyright.htm for details.

  This software is distributed WITHOUT ANY WARRANTY; without even 
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
  PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#ifndef AlbatrossDataSource_h
#define AlbatrossDataSource_h

#include "DataSource.h"

namespace OpenGC
{

class AlbatrossDataSource : public DataSource
{
	public:
		AlbatrossDataSource();
		~AlbatrossDataSource();

		// The money function
		bool OnIdle();
};

} // end namespace OpenGC

#endif
