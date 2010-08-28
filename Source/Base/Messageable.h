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

#ifndef Messageable_h
#define Messageable_h

#include "Globals.h"
#include "Messages.h"

namespace OpenGC {

/** Abstract base class for objects that can receive messages (i.e. are
 * messageable). Inherit from it and implement OnMessage() to receive
 * messages. */
class Messageable 
{
public:
	/** When a Messageable is constructed, add it to the global list
	 * of messageable objects */
	Messageable() { globals->m_MessageableList->Add(this); }
	
	/** When a Messageable is destructed, remove it from the global list
	 * of messageable objects */
	virtual ~Messageable() { globals->m_MessageableList->Remove(this); }
	
	/** Called when a message is triggered somewhere */
	virtual void OnMessage(Message message, void *data) = 0;
};

} // end namespace OpenGC

#endif

