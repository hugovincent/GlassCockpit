/*=========================================================================

  OpenGC - The Open Source Glass Cockpit Project
  Please see our web site at http://www.opengc.org

  Albatross UAV Project - http://www.albatross-uav.org

  Copyright (c) 2001-2003 Damion Shelton
  All rights reserved. Contributions by Hugo Vincent.
  See Copyright.txt or http://www.opengc.org/Copyright.htm for details.

  This software is distributed WITHOUT ANY WARRANTY; without even 
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
  PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include <stdio.h>
#include <string.h>
#include "DataSource.h"

namespace OpenGC
{

DataSource::DataSource()
{
	m_Airframe = new AirframeDataContainer;
}

DataSource::~DataSource()
{
	delete m_Airframe;
}

// Initialize the data
void DataSource::InitializeData()
{
	m_Airframe->InitializeData();
}

bool DataSource::OnIdle()
{
	// The default version does nothing
	return false;
}

bool DataSource::Open()
{
	// The default version returns false, which doesn't necessarily mean anything
	return false;
}

} // end namespace OpenGC
