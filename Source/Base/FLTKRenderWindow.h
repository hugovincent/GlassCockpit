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

/**
 * FLTK implementation of the OpenGC render window
 */

#ifndef FLTKRenderWindow_h
#define FLTKRenderWindow_h

#if 0
#include <FL/Fl_Gl_Window.H>
#include "RenderWindow.h"

namespace OpenGC
{

class FLTKRenderWindow : public Fl_Gl_Window, public RenderWindow
{
	public:

		/** Constructor follows FLTK syntax */
		FLTKRenderWindow(int X, int Y, int W, int H, const char *L = 0);

		/** Yawn, a destructor */
		~FLTKRenderWindow();

		/** FL rendering callback */
		void draw();

		/**  FL event handling callback */
		int handle(int);

		/**  Render the entire window */
		void Render();
};

} // end namespace OpenGC

#endif
#endif
