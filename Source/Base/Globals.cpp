// FIXME header

#include "Globals.h"

namespace OpenGC {

Globals::Globals()
{
	m_PrefManager = new PrefManager();
	m_NavDatabase = new NavDatabase();
	m_FontManager = new FontManager();
	m_MessageableList = new MessageableList();
	m_DataSource = 0;
}

Globals::~Globals()
{
	if(m_FontManager != 0)
	{
		delete m_FontManager;
		m_FontManager = 0;
	}

	if (m_DataSource != 0)
	{
		delete m_DataSource;
		m_DataSource = 0;
	}

	if (m_PrefManager != 0)
	{
		delete m_PrefManager;
		m_PrefManager = 0;
	}
	
	if (m_NavDatabase != 0)
	{
		delete m_NavDatabase;
		m_NavDatabase = 0;
	}

	if (m_MessageableList != 0)
	{
		delete m_MessageableList;
		m_MessageableList = 0;
	}
}
	
} // end namespace OpenGC
