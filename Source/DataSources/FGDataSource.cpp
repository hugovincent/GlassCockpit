/*=========================================================================

  OpenGC - The Open Source Glass Cockpit Project
  Please see our web site at http://www.opengc.org

  Albatross UAV Project - http://www.albatross-uav.org

  Copyright (c) 2001-2003 Damion Shelton
  Copyright (c) 2006 Hugo Vincent
  All rights reserved.

  This file is distributed under the terms of the GNU General Public
  License version 2.0. See included COPYING file for details.

=========================================================================*/

/**
 * Sets up the data class structure and methods for receiving a UDP data
 * packet transmitted by the simulation program.
 *
 * This program acts as a "server" receiving data from the flight simulation
 */

#include <stdio.h>
#include <iostream>
#include "FGDataSource.h"
#include "Constants.h"
#include "Globals.h"
#include "Debug.h"

namespace OpenGC {

// Constructor
FGDataSource::FGDataSource()
{ 
	// Initialize the data, but we're going to delay opening the sockets until all
	// the other data files and command line arguments have been read
	this->InitializeData();

	// Set FlightGear buffer length is
	m_BufferLength = sizeof(FGData);

	// Create the buffer
	m_Buffer = new char[m_BufferLength];

	// Temp buffer used to extract message
	m_TempMsg = new char[m_BufferLength];
}

FGDataSource::~FGDataSource()
{
//	m_Socket.close();
	delete[] m_Buffer;
	delete[] m_TempMsg;
}

bool FGDataSource::Open()
{
	// Get the host:port string from the Config system and parse it
	m_Host = globals->m_PrefManager->GetPrefS("FlightGearHost");
	m_ReceivePort = globals->m_PrefManager->GetPrefI("FlightGearPort");
	if (true)
	{
		printf("FGDataSourse: host \"%s\", port %d\n", 
				m_Host.c_str(), m_ReceivePort);
	}
	else
	{
		std::cerr << "FGDataSource: invalid configuration.\n";
		return false;
	}
	
	if (m_ReceivePort < 1025 || m_ReceivePort > 65535)
	{
		std::cerr << "FGDataSource: invalid port number.\n";
		return false;
	}

#if 0
	// Must call this before any other net stuff
	netInit();

	// We'll assume the connection is valid until proved otherwise
	m_ValidConnection = true;

	// Try to open a socket
	if (!m_Socket.open(false))
	{
		printf("FlightGear data source: error opening socket\n");
		m_ValidConnection = false;
		return false;
	}

	m_Socket.setBlocking(false);

	if (m_Socket.bind(m_Host.c_str(), m_ReceivePort) == -1)
	{
		printf("FGDataSource: error binding to port %d\n", m_ReceivePort);
		m_ValidConnection = false;
		return false;
	}
#endif
	return true;
}

bool FGDataSource::GetData()
{
	if (!m_ValidConnection)
		return false;

#if 0
	// Length of the message received from Flightgear
	int receivedLength = 0;

	// Actual length after flushing accumulated messages
	int finalReceivedLength = 0;

	// The code in the "do" block flushes the buffer so that only
	// the most recent message remains. This eliminates the build-up
	// of old messages in the network buffer (which we don't directly
	// control)
	do
	{
		receivedLength = m_Socket.recv(m_TempMsg, m_BufferLength, 0);

		if(receivedLength >= 0)
		{
			for(int i = 0; i < m_BufferLength; i++)
				m_Buffer[i] = m_TempMsg[i];

			finalReceivedLength = receivedLength;
		}
	} while(receivedLength >=0);

	// At this point, m_Buffer[] contains the most recent message
	if(finalReceivedLength>0)
	{
		assert(finalReceivedLength == sizeof(FGData)); // possible superfluous
		m_FDM = (FGData*) m_Buffer;
		return true;
	}
	else
		return false;
#endif
	return false;
}

bool FGDataSource::OnIdle()
{
   	// Abort if the connection isn't valid
   	if( !m_ValidConnection )
   		return false;

   	// Input data from the LAN socket
   	if ( !GetData() )
   		return false;

#if 0
   	// keep in sync with FG changes
   	assert(m_FDM->version_id == 4);

	m_Airframe->SetLatitude(m_FDM->latitude);
	m_Airframe->SetLongitude(m_FDM->longitude);
	
	m_Airframe->SetRoll(m_FDM->bank);
	m_Airframe->SetPitch(m_FDM->pitch);
	m_Airframe->SetTrue_Heading(m_FDM->heading);
	m_Airframe->SetMag_Variation(m_FDM->magvar * RAD_TO_DEG);
	double magHeading = m_FDM->heading - m_Airframe->GetMag_Variation();
	if (magHeading < 0.0 )
		magHeading += 360.0;
	m_Airframe->SetMag_Heading(magHeading);

	m_Airframe->SetPressure_Alt_Feet(m_FDM->altitude);
	//AGL_Alt_Feet = m_FDM->altitude_agl;

	m_Airframe->SetIAS(m_FDM->v_kcas); // calibrated airspeed
	//EAS = m_FDM->v_eas; // equivalent airspeed
	
	// FIXME this is old commented out code:
	//TAS = EAS * (1.0 + (Barometric_Alt_Feet * 0.000017));
	//= TAS - (cos((Mag_Heading - wind_direction)/57.29456) * wind_velocity);

	m_Airframe->SetVertical_Speed_FPM(m_FDM->vvi * 60.0); // FlightGear units are feet per second
	m_Airframe->SetGround_Speed_K(m_FDM->groundspeed * 3600 / METERS_TO_FEET * METER_PER_NM);

#endif
	
#if 0
	// FIXME enable other fields
	// Get euler angles and rates
	Alpha = m_FDM->alpha;
	Alpha_Dot = m_FDM->alpha_dot;
	Beta = m_FDM->beta;
	Beta_Dot = m_FDM->beta_dot;
	Phi_Dot = m_FDM->phi_dot;
	Psi_Dot = m_FDM->psi_dot;
	Theta_Dot = m_FDM->theta_dot;  

	// get the position of the control surfaces
	Flaps_Deflection = m_FDM->flaps;      
	Elevator_Deflection = m_FDM->elevator;
	Right_Aileron_Deflection = m_FDM->right_aileron;
	Left_Aileron_Deflection = m_FDM->left_aileron;
	Rudder_Deflection = m_FDM->rudder;

	// get the position of the engine controls
	Throttle = m_FDM->throttle[0];
	Mixture = m_FDM->mixture[0];
	Prop_Advance = m_FDM->prop_advance[0];

	// For single and twin engine aircraft, use the first and second elements of each set
	N1 = m_FDM->n1_turbine[0];
	N2 = m_FDM->n2_turbine[0];
	EGT = m_FDM->egt[0];
	EPR = m_FDM->epr[0];
	Fuel_Flow_PPH = m_FDM->fuel_flow[0];
	Oil_Pressure = m_FDM->oil_pressure[0];
#endif
	return false;
}

} // end namespace OpenGC
