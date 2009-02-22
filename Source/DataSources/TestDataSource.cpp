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
#include "Debug.h"
#include "TestDataSource.h"

namespace OpenGC
{

	TestDataSource::TestDataSource()
	{
		this->InitializeData();

		// Set initial state
		m_Airframe->SetStatus_Active(false);
		m_Airframe->SetDirector_Active(false);
		m_Airframe->SetGot_Data(false);
		m_Airframe->SetStatus_Text_NoData("WAITING FOR CONNECTION");
		m_State = STATE_WAITING;

		m_Airframe->SetStatus_Text1("YAW");
		m_Airframe->SetStatus_Text2("ALT");
		m_Airframe->SetStatus_Text3("BNK");
		m_Airframe->SetStatus_Text4("VEL");
		m_Airframe->SetStatus_Colour1(1);
		m_Airframe->SetStatus_Colour2(1);
		m_Airframe->SetStatus_Colour3(1);
		m_Airframe->SetStatus_Colour4(1);
	}

	TestDataSource::~TestDataSource()
	{
		// Deconstructor
	}

	bool TestDataSource::OnIdle()
	{
		// ++++++ Generate Data +++++++++++++++++++++++++++++++++++++++++++++++++++
		static double t = 0.0;
		t += 1.0 / 24.0;

		switch (m_State) { 
			case STATE_WAITING: {
				if (t >= 1.0) {
					m_Airframe->SetStatus_Text_NoData("CONNECTED (TEST)");
					m_State = STATE_CONNECTING;
					LogPrintf("TestDataSource: Connected (test mode).\n");
					return true;
				}
				else
					return false;
			}
			case STATE_CONNECTING: {
				if (t >= 1.5) {
					m_Airframe->SetBarometric_Pressure(1010.0); // hPa ambient
					m_Airframe->SetGPS_Mode(3);
					m_Airframe->SetGPS_Sats(5);
					m_Airframe->SetGot_Data(true);
					m_Airframe->SetStatus_Active(true);
					m_State = STATE_FD;
					LogPrintf("TestDataSource: Activated status display.\n");
				}
				return false;
			}
			case STATE_FD: {
					if (t >= 5.0) {
					    m_Airframe->SetStatus_Colour1(2);
					    m_Airframe->SetStatus_Colour2(2);
					    m_Airframe->SetStatus_Colour3(2);
					    m_Airframe->SetStatus_Colour4(2);
					    m_Airframe->SetDirector_Active(true);
					    m_State = STATE_RUNNING;
					    LogPrintf("TestDataSource: Flight Director on.\n");
					}
					break;
				}
			case STATE_RUNNING:
				break;
		}

		double roll = 0.3 * sin(t) * 180 / M_PI;
		double pitch = 0.15 * cos(t) * 180 / M_PI;
		double heading = fmodf(t * 15.0, 360.0);
		//double trueHeading = fmodf((t * 15) + 2 * sin(t), 360.0);
		//double heading = 60.0 * sin(t/4);
		if (heading < 0.0)
			heading += 360.0;
		double airspeed = t * 4;
		double vertSpeed = 200 * sin(t/2 + 0.5); // should be calculated
		//double alt = 200 + (30 * sin(t/6));
		double alt = t * 15;

		// ++++++ Update Data +++++++++++++++++++++++++++++++++++++++++++++++++++++
		// -------Orientation--------------
		m_Airframe->SetRoll(roll);
		m_Airframe->SetPitch(pitch);
		m_Airframe->SetTrue_Heading(heading);
		m_Airframe->SetTrack_Heading(360 - heading);

		// -------Speeds-------------------
		m_Airframe->SetAirspeed_KT(airspeed);
		m_Airframe->SetVertical_Speed_FPM(vertSpeed);
		m_Airframe->SetGround_Speed_MS(0.0); // Knots

		// -------Accelerations------------
		m_Airframe->SetAccel_Body_Fwd(0.0);
		m_Airframe->SetAccel_Body_Right(0.0);
		m_Airframe->SetAccel_Body_Down(1.0);

		// -------Altitude-----------------
		m_Airframe->SetAltitude_MSL_Feet(alt);
		m_Airframe->SetAltitude_AGL_Feet(0.0);
		m_Airframe->SetBarometric_Pressure(1010.0);

		// -------Position-----------------
		m_Airframe->SetLatitude(-43.479 + 0.005 * t); // }
		m_Airframe->SetLongitude(172.523);            // } Christchurch
//		m_Airframe->SetLatitude(37.621134 + 0.005 * t); // }
//		m_Airframe->SetLongitude(-122.374530);          // } San Francisco
		
		// -------Engine-------------------
		m_Airframe->SetEngine_RPM(4500.0 + 4500.0 * sin(t/2.2));
		m_Airframe->SetEngine_CHT(125.0 + 125.0 * sin(t/2.5));
		m_Airframe->SetEngine_EGT(500.0 + 500 * sin (t/3));
		m_Airframe->SetEngine_Mixture(8.0 + 8.0 * sin(t/2));
		m_Airframe->SetVoltage_Alternator(0.0);
		m_Airframe->SetVoltage_Battery(0.0);

		// -------Extra Stuff--------------
		m_Airframe->SetInternal_Temp(30 + 2*sin(t / 100.0));
		m_Airframe->SetExternal_Temp(10 - 2*sin(t / 100.0));
		m_Airframe->SetWind_Speed(10.0);
		m_Airframe->SetWind_Direction(360.0 - heading);

		// -------Flight Director---------- (aka autopilot)
		m_Airframe->SetDirector_Pitch(5.0);
		m_Airframe->SetDirector_Roll(20.0);
		m_Airframe->SetDirector_Heading(10.0);
		m_Airframe->SetDirector_Altitude(400.0);
		m_Airframe->SetDirector_Airspeed(80.5);

		return true;
	}

} // end namespace OpenGC

