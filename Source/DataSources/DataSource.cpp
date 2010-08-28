/*=========================================================================

  Copyright (c) 2001-2004 Damion Shelton
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
