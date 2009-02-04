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

#include <math.h>

#include "Globals.h"
#include "CalcManager.h"

namespace OpenGC {

CalcManager::CalcManager()
{
}

CalcManager::~CalcManager()
{
}

void CalcManager::InitFromXMLNode(XMLNode calcNode)
{

}
	
bool CalcManager::Calculate()
{
	AirframeDataContainer* data = globals->m_DataSource->GetAirframe();
	
	// FIXME this isn't really the right place for this...
	double lat = data->GetLatitude();
	double lon = data->GetLongitude();
	CoursePoint p = CoursePoint(lat, lon);
	globals->m_NavDatabase->GetFlightCourse()->push_back(p);
	
	return false;
}
	
} // end namespace OpenGC
