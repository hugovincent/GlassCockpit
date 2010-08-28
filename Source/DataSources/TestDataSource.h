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

#ifndef TestDataSource_h
#define TestDataSource_h

#include "DataSource.h"

namespace OpenGC
{

typedef enum {STATE_WAITING, STATE_CONNECTING, STATE_FD, STATE_RUNNING} TestState; 

class TestDataSource : public DataSource
{
	public:
		TestDataSource();
		~TestDataSource();

		// The money function
		bool OnIdle();

	private:
		TestState m_State;
};

} // end namespace OpenGC

#endif
