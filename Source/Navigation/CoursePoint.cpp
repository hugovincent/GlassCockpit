/*=========================================================================

  OpenGC - The Open Source Glass Cockpit Project
  Please see our web site at http://www.opengc.org

  Albatross UAV Project - http://www.albatross-uav.org

  Copyright (c) 2006 Hugo Vincent
  All rights reserved.
  See Copyright.txt or http://www.opengc.org/Copyright.htm for details.

  This software is distributed WITHOUT ANY WARRANTY; without even 
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
  PURPOSE.  See the above copyright notice for more information.

  =========================================================================*/

#include <cmath>
#include "CoursePoint.h"

namespace OpenGC
{

using namespace std;

CoursePoint::CoursePoint()
{

}

CoursePoint::CoursePoint(double lat, double lon)
{
	m_Lat = lat;
	m_Lon = lon;
}

CoursePoint::~CoursePoint()
{

}

void CoursePoint::GetCoords(double &lat, double &lon)
{
	lat = m_Lat;
	lon = m_Lon;
}

} // end namespace OpenGC

