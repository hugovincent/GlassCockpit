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

#include "AlbatrossDataSource.h"
#include "Constants.h"
#include "Debug.h"

namespace OpenGC {

AlbatrossDataSource::AlbatrossDataSource()
{
	this->InitializeData();
	m_Airframe->SetStatus_Active(false);
	m_Airframe->SetDirector_Active(false);
	m_Airframe->SetGot_Data(false);
	m_Airframe->SetStatus_Text_NoData("WAITING FOR CONNECTION");

	m_Airframe->SetStatus_Text1("YAW");
	m_Airframe->SetStatus_Text2("ALT");
	m_Airframe->SetStatus_Text3("BNK");
	m_Airframe->SetStatus_Text4("VEL");
	m_Airframe->SetStatus_Colour1(1);
	m_Airframe->SetStatus_Colour2(1);
	m_Airframe->SetStatus_Colour3(1);
	m_Airframe->SetStatus_Colour4(1);

	LogPrintf("AlbatrossDataSource: NOT CONNECTED\n");
}

AlbatrossDataSource::~AlbatrossDataSource()
{
	// Deconstructor
}

bool AlbatrossDataSource::OnIdle()
{
	// FIXME actually get data (duh!)

	return false; // should be true once data is coming in
}

} // end namespace OpenGC

