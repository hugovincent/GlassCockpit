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

