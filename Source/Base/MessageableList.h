// FIXME header

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

