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

#ifndef TestDataSource_h
#define TestDataSource_h

#include "DataSource.h"

namespace OpenGC
{

typedef enum {STATE_WAITING, STATE_CONNECTING, STATE_FD, STATE_RUNNING} TestState; 

class TestDataSource : public DataSource
{
	public:
		TestDataSource();
		~TestDataSource();

		// The money function
		bool OnIdle();

	private:
		TestState m_State;
};

} // end namespace OpenGC

#endif
