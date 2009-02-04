// FIXME header

#ifndef Globals_h
#define Globals_h

#include "DataSource.h"
#include "NavDatabase.h"
#include "PrefManager.h"
#include "FontManager.h"
#include "MessageableList.h"

namespace OpenGC {

class Globals 
{
public:
	Globals();
	~Globals();

	DataSource		*m_DataSource;
	NavDatabase		*m_NavDatabase;
	PrefManager		*m_PrefManager;
	FontManager		*m_FontManager;
	MessageableList	*m_MessageableList;
};

extern Globals *globals;

} // end namespace OpenGC

#endif
