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

#include "Globals.h"

namespace OpenGC {

Globals::Globals()
{
	m_PrefManager = new PrefManager();
	m_NavDatabase = new NavDatabase();
	m_FontManager = new FontManager();
	m_CircleEvaluator = new CircleEvaluator();
	m_RasterMapManager = new RasterMapManager();
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

	if (m_RasterMapManager != 0)
	{
		delete m_RasterMapManager;
		m_RasterMapManager = 0;
	}

	if (m_CircleEvaluator != 0)
	{
		delete m_CircleEvaluator;
		m_CircleEvaluator = 0;
	}

	if (m_MessageableList != 0)
	{
		delete m_MessageableList;
		m_MessageableList = 0;
	}
}
	
} // end namespace OpenGC
