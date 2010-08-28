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

/**
 * This header addresses the various ways that OpenGL headers
 * are handled on different systems
 */

#ifndef _WIN32
	// Non windows, doesn't require Windows header to work correctly
	#ifndef __gl_h_
		#ifdef __APPLE_CC__
			#include <OpenGL/gl.h>
			#include <OpenGL/glu.h>
		#else
			#include <GL/gl.h>
			#include <GL/glu.h>
		#endif                
	#endif

	// Required for compatibility with glext.h style function definitions
	#ifndef APIENTRY
		#define APIENTRY
	#endif

#else	// Win32 Platform

	#ifdef _WIN32 
		#define WIN32_LEAN_AND_MEAN 
		#include <windows.h> 
	#endif

	#include <GL/gl.h>
	#include <GL/glu.h>

#endif	//	 WIN32
