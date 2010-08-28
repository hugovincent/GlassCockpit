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

#ifndef MessageableList_h
#define MessageableList_h

#include <list>
#include "Messages.h"

namespace OpenGC {

// Forward declaration to avoid circular dependecy
class Messageable;

/** A list of messageable objects in OpenGC */
class MessageableList : std::list<Messageable*>
{
public:
	MessageableList();
	~MessageableList();

	/** Add an object to the MessageableList */
	void Add(Messageable *obj);
	
	/** Remove an object from the MessageableList */
	void Remove(Messageable *obj);
	
	/** Send a message to all Messageable objects */
	void DispatchMessage(Message message, void *data);
};

} // end namespace OpenGC

#endif

