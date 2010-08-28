/*=========================================================================

  Copyright (c) 2001-2004 Damion Shelton
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

#include "MessageableList.h"
#include "Messageable.h"

namespace OpenGC {

MessageableList::MessageableList() {}
MessageableList::~MessageableList() {}

void MessageableList::Add(Messageable *obj)
{
	this->push_back(obj);
}

void MessageableList::Remove(Messageable *obj)
{
	MessageableList::iterator iter;
	for (iter = this->begin(); iter != this->end(); ++iter)
	{
		if ((*iter) == obj)
		{
			this->remove(*iter);
			break;
		}
	}
}

void MessageableList::DispatchMessage(Message message, void *data)
{
	MessageableList::iterator it;
	for (it = this->begin(); it != this->end(); ++it)
	{
		(*it)->OnMessage(message, data);
	}
}

}; // end namespace OpenGC

