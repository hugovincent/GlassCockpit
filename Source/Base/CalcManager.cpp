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
