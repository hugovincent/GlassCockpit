/*=========================================================================

  OpenGC - The Open Source Glass Cockpit Project
  Please see our web site at http://www.opengc.org

  Albatross UAV Project - http://www.albatross-uav.org

  Copyright (c) 2001-2003 Damion Shelton
  All rights reserved. Contributions by Hugo Vincent.
  See Copyright.txt or http://www.opengc.org/Copyright.htm for details.

  This software is distributed WITHOUT ANY WARRANTY; without even 
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
  PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#ifndef DataSourceMacro_h
#define DataSourceMacro_h

/** Macro for data members which allow non-source write operations */
#define DataMacro(name,type) \
	public: \
		type Get##name() \
		{ \
			return m_##name; \
		} \
		void Set##name(type value) \
		{ \
			m_##name = value; \
		} \
	protected: \
		type m_##name;

#endif

