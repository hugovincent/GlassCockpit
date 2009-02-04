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

#include <iostream>
#include <math.h>

#include <FL/Fl.H>
#include "FLTKRenderWindow.h"

namespace OpenGC
{

FLTKRenderWindow::FLTKRenderWindow(int X, int Y, int W, int H, const char *L) : Fl_Gl_Window(X, Y, W, H, L)
{
	// Safe to enable rendering
	m_IsOKToRender = true;
}

FLTKRenderWindow::~FLTKRenderWindow()
{
}

void FLTKRenderWindow::draw()
{
	this->Render();
}

int FLTKRenderWindow::handle(int event)
{
	switch(event) {
		case FL_PUSH:		// Mouse down
			{
				this->CallBackMouseFunc(Fl::event_button(), 0,
						Fl::event_x(), Fl::event_y() );
				return 1;
			}
		case FL_DRAG:		// Mouse drag
			{
				return 1;
			}
		case FL_RELEASE:	// Mouse up
			{
				this->CallBackMouseFunc(Fl::event_button(), 1, 
						Fl::event_x(), Fl::event_y() );
				return 1;
			}
		case FL_FOCUS :
		case FL_UNFOCUS :
			{
				// return 1 if you want keyboard events, 0 otherwise
				return 1;
			}
		case FL_KEYDOWN:	// Keyboard key pressed
			{
				int key = Fl::event_key();
				if (key < 128 && key >= 32)
					this->CallBackKeyboardFunc(key, Fl::event_state() >> 16);
				return 1;
			}
		default:
			{
				// pass other events to the base class, e.g. Alt+F4
				return Fl_Gl_Window::handle(event);
			}
	}
}

void FLTKRenderWindow::Render()
{
	// Check to see if any FL window stuff has changed
	if (!valid())
	{
		this->Resize(w(), h());
	}

	// Call the base class render to do everything else
	RenderWindow::Render();
}

} // end namespace OpenGC

