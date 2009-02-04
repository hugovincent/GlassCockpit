// FIXME header

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

