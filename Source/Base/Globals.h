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

#ifndef Globals_h
#define Globals_h

#include "DataSource.h"
#include "NavDatabase.h"
#include "PrefManager.h"
#include "FontManager.h"
#include "RasterMapManager.h"
#include "CircleEvaluator.h"
#include "MessageableList.h"

namespace OpenGC {

class Globals 
{
public:
	Globals();
	~Globals();

	DataSource			*m_DataSource;
	NavDatabase			*m_NavDatabase;
	PrefManager			*m_PrefManager;
	FontManager			*m_FontManager;
	RasterMapManager	*m_RasterMapManager;
	CircleEvaluator		*m_CircleEvaluator;
	MessageableList		*m_MessageableList;
};

extern Globals *globals;

} // end namespace OpenGC

#endif
