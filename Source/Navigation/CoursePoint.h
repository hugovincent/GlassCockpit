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

#ifndef CoursePoint_h
#define CoursePoint_h

namespace OpenGC
{

class CoursePoint
{
public:
	CoursePoint();
	CoursePoint(double lat, double lon);
	~CoursePoint();

	void GetCoords(double &lat_, double &lon_);

private:
	double m_Lat, m_Lon;
};
	
} // end namespace OpenGC

#endif

