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
	DataMacro(Roll, double);

	/** Pitch in degrees from horizontal, pitch up + */
	DataMacro(Pitch, double);

	/** True heading in degrees */
	DataMacro(True_Heading, double);

	/** Track over ground heading in degrees */
	DataMacro(Track_Heading, double);

	/** Latitude in degrees (with fractional degrees) North +, South - */
	DataMacro(Latitude, double);

	/** Longitude in degrees (with fractional degrees) East +, West - */
	DataMacro(Longitude, double); 

	/** Accelerations in body frame (used for turn coordinator), G's. */
	DataMacro(Accel_Body_Fwd, double);
	DataMacro(Accel_Body_Right, double);
	DataMacro(Accel_Body_Down, double);

	//
	//--------------Speed--------------- 
	//

	/** True airspeed in knots */
	DataMacro(Airspeed_KT, double);

	/** Ground speed in meters/sec */
	DataMacro(Ground_Speed_MS, double);

	/** Rate of climb or descent, in feet per minute */
	DataMacro(Vertical_Speed_FPM, double);

	//
	//------------Altitude---------------
	//

	/** Altitude in feet above ground level */
	DataMacro(Altitude_AGL_Feet, double);

	/** Altitude in feet above sea level */
	DataMacro(Altitude_MSL_Feet, double);

	/** Barometric pressure hPa */
	DataMacro(Barometric_Pressure, double);

	//
	//--------------Engine Parameters-----------------
	//

	/** Engine revolutions per minute */
	DataMacro(Engine_RPM, double);

	/** Engine cylinder head temperature (deg. C) */
	DataMacro(Engine_CHT, double);

	/** Engine exhaust gas temperature (deg. C) */
	DataMacro(Engine_EGT, double);

	/** Engine mixture (as a percentage, 0 for lean, 100 for rich) */
	DataMacro(Engine_Mixture, double);

	/** Alternator voltage */
	DataMacro(Voltage_Alternator, double);

	/** Battery voltage */
	DataMacro(Voltage_Battery, double);

	//
	//--------------Flight Director-------------------
	//

	/** Flight director Active */
	DataMacro(Director_Active, bool);

	/** Flight director Bank */
	DataMacro(Director_Roll, double);

	/** Flight director Pitch */
	DataMacro(Director_Pitch, double);

	/** Flight director Heading */
	DataMacro(Director_Heading, double);

	/** Flight director Altitude */
	DataMacro(Director_Altitude, double);

	/** Flight director Airspeed */
	DataMacro(Director_Airspeed, double);

	/** Flight director Vertical Speed (FPM) */
	DataMacro(Director_Vertical_Speed, double);

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
	DataMacro(Internal_Temp, double);

	/** Albatross external temperature */
	DataMacro(External_Temp, double);

	/** Albatross wind direction estimate */
	DataMacro(Wind_Direction, double);

	/** Albatross wind speed estimate */
	DataMacro(Wind_Speed, double);

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

	public:

	void InitializeData();

	AirframeDataContainer();
	virtual ~AirframeDataContainer();

	protected:

};

} // end namespace OpenGC
#endif
