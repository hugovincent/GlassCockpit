/*=========================================================================

  OpenGC - The Open Source Glass Cockpit Project
  Please see our web site at http://www.opengc.org

  Albatross UAV Project - http://www.albatross-uav.org

  Copyright (c) 2001-2003 Damion Shelton
  All rights reserved.
  See Copyright.txt or http://www.opengc.org/Copyright.htm for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#ifndef FGDataSource_h
#define FGDataSource_h

#include "DataSource.h"

#include <string>
//#include <plib/netSocket.h>
#include "FlightGear_Protocol.h"

using namespace std;

namespace OpenGC
{

/** This is the actual DataSource */
class FGDataSource : public DataSource
{
	public:

		FGDataSource();  
		virtual ~FGDataSource();

		// Open the network connection
		bool Open();

		// The "money" function
		bool OnIdle(); 

	protected:

		// Get data from FlightGear
		bool GetData();

		// Flight model data received from FlightGear
		FGData *m_FDM;

		// The socket
//		netSocket m_Socket;

		// The host and port we're receiving on
		string m_Host;
		int m_ReceivePort;

		// Are we connected to Flightgear?
		bool m_ValidConnection;

		// The message received from Flightgear
		char *m_Buffer;

		// A temporary buffer used in the receive process
		char *m_TempMsg;

		// Maximum length of the buffer
		int m_BufferLength;
};

} // end namespace OpenGC


#endif
