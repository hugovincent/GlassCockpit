// FIXME header

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

