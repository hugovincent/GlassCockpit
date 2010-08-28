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

#include "AirframeDataContainer.h"

namespace OpenGC
{

AirframeDataContainer::AirframeDataContainer()
{
	this->InitializeData();
}

AirframeDataContainer::~AirframeDataContainer()
{

}

void
	AirframeDataContainer
	::InitializeData()
	{
		m_Roll = 0;
		m_Pitch = 0;
		m_True_Heading = 0;
		m_Track_Heading = 0;

		m_Latitude = 0;
		m_Longitude = 0;

		m_Accel_Body_Fwd = 0;
		m_Accel_Body_Right = 0;
		m_Accel_Body_Down = 0;

		m_Airspeed_KT = 0;
		m_Ground_Speed_MS = 0;
		m_Vertical_Speed_FPM = 0;

		m_Altitude_AGL_Feet = 0;
		m_Altitude_MSL_Feet = 0;
		m_Barometric_Pressure = 1010.0;

		m_Engine_RPM = 0.0;
		m_Engine_CHT = 0.0;
		m_Engine_EGT = 0.0;
		m_Engine_Mixture = 0.0;
		m_Voltage_Alternator = 0.0;
		m_Voltage_Battery = 0.0;

		m_Director_Active = false;
		m_Director_Pitch = 0.0;
		m_Director_Roll = 0.0;
		m_Director_Heading = 0.0;
		m_Director_Altitude = 0.0;
		m_Director_Airspeed = 0.0;
		m_Director_Vertical_Speed = 0.0;

		m_Got_Data = false;
		m_Status_Active = false;
		m_GPS_Mode = 0;
		m_GPS_Sats = 0;
		m_Internal_Temp = 0.0;
		m_External_Temp = 0.0;

		m_Wind_Speed = 0.0;
		m_Wind_Direction = 0.0;

		m_Status_Text1 = 0; // Strings set to null pointers
		m_Status_Text2 = 0;
		m_Status_Text3 = 0;
		m_Status_Text4 = 0;
		m_Status_Colour1 = 0;
		m_Status_Colour2 = 0;
		m_Status_Colour3 = 0;
		m_Status_Colour4 = 0;
		m_Status_Text_NoData = 0;
	}

} // end namespace OpenGC

