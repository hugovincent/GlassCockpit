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

#ifndef AirframeDataContainer_h
#define AirframeDataContainer_h

#include "DataContainer.h"

namespace OpenGC
{

class AirframeDataContainer : public DataContainer
{

	//
	//------------Heading and Location-------------
	//

	/** Roll in degrees around the axis of flight, right roll + */
	DataMacro(Roll, float);

	/** Pitch in degrees from horizontal, pitch up + */
	DataMacro(Pitch, float);

	/** True heading in degrees */
	DataMacro(True_Heading, float);

	/** Track over ground heading in degrees */
	DataMacro(Track_Heading, float);

	/** Latitude in degrees (with fractional degrees) North +, South - */
	DataMacro(Latitude, double);

	/** Longitude in degrees (with fractional degrees) East +, West - */
	DataMacro(Longitude, double); 

	/** Accelerations in body frame (used for turn coordinator), G's. */
	DataMacro(Accel_Body_Fwd, float);
	DataMacro(Accel_Body_Right, float);
	DataMacro(Accel_Body_Down, float);

	//
	//--------------Speed--------------- 
	//

	/** True airspeed in knots */
	DataMacro(Airspeed_KT, float);

	/** Ground speed in meters/sec */
	DataMacro(Ground_Speed_MS, float);

	/** Rate of climb or descent, in feet per minute */
	DataMacro(Vertical_Speed_FPM, float);

	//
	//------------Altitude---------------
	//

	/** Altitude in feet above ground level */
	DataMacro(Altitude_AGL_Feet, float);

	/** Altitude in feet above sea level */
	DataMacro(Altitude_MSL_Feet, float);

	/** Barometric pressure hPa */
	DataMacro(Barometric_Pressure, float);

	//
	//--------------Engine Parameters-----------------
	//

	/** Engine revolutions per minute */
	DataMacro(Engine_RPM, float);

	/** Engine cylinder head temperature (deg. C) */
	DataMacro(Engine_CHT, float);

	/** Engine exhaust gas temperature (deg. C) */
	DataMacro(Engine_EGT, float);

	/** Engine mixture (as a percentage, 0 for lean, 100 for rich) */
	DataMacro(Engine_Mixture, float);

	/** Alternator voltage */
	DataMacro(Voltage_Alternator, float);

	/** Battery voltage */
	DataMacro(Voltage_Battery, float);

	//
	//--------------Flight Director-------------------
	//

	/** Flight director Active */
	DataMacro(Director_Active, bool);

	/** Flight director Bank */
	DataMacro(Director_Roll, float);

	/** Flight director Pitch */
	DataMacro(Director_Pitch, float);

	/** Flight director Heading */
	DataMacro(Director_Heading, float);

	/** Flight director Altitude */
	DataMacro(Director_Altitude, float);

	/** Flight director Airspeed */
	DataMacro(Director_Airspeed, float);

	/** Flight director Vertical Speed (FPM) */
	DataMacro(Director_Vertical_Speed, float);

	//
	//--------------Albatross Specific Stuff----------
	//

	/** Do we have incoming data */
	DataMacro(Got_Data, bool);

	/** Status indicator panel Active */
	DataMacro(Status_Active, bool);

	/** Albatross GPS Mode */
	DataMacro(GPS_Mode, int);

	/** Albatross GPS Number of Satellites in view */
	DataMacro(GPS_Sats, int);

	/** Albatross internal temperature */
	DataMacro(Internal_Temp, float);

	/** Albatross external temperature */
	DataMacro(External_Temp, float);

	/** Albatross wind direction estimate */
	DataMacro(Wind_Direction, float);

	/** Albatross wind speed estimate */
	DataMacro(Wind_Speed, float);

	/** Albatross status text strings (max 3 chars) */
	DataMacro(Status_Text1, char *);
	DataMacro(Status_Text2, char *);
	DataMacro(Status_Text3, char *);
	DataMacro(Status_Text4, char *);

	/** Albatross status text colour (0 = off, 1 = red, 2 = green) */
	DataMacro(Status_Colour1, int);
	DataMacro(Status_Colour2, int);
	DataMacro(Status_Colour3, int);
	DataMacro(Status_Colour4, int);

	/** Text area for when we have no incoming data */
	DataMacro(Status_Text_NoData, char *);
	
	/** Time stamp */
	DataMacro(TimeStamp, double);

	public:

	void InitializeData();

	AirframeDataContainer();
	virtual ~AirframeDataContainer();

	protected:

};

} // end namespace OpenGC
#endif
