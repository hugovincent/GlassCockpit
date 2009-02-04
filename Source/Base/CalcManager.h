/*=========================================================================

  OpenGC - The Open Source Glass Cockpit Project
  Please see our web site at http://www.opengc.org

  Albatross UAV Project - http://www.albatross-uav.org

  Copyright (c) 2001-2003 Hugo Vincent.
  All rights reserved.
  See Copyright.txt or http://www.opengc.org/Copyright.htm for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#ifndef CalcManager_h
#define CalcManager_h

#include "DataSource.h"
#include "XMLNode.h"

namespace OpenGC {

class CalcManager
{
public:
	CalcManager();
	~CalcManager();
	
	void InitFromXMLNode(XMLNode calcNode);
	bool Calculate();
};

} // end namespace OpenGC

#endif
