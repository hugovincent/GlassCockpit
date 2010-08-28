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

#include <stdio.h>
#include <math.h>
#include <unistd.h>

#include "Globals.h"
#include "GLHeaders.h"
#include "Gauge.h"
#include "RenderWindow.h"

namespace OpenGC
{

RenderWindow::RenderWindow()
{
	// Set the window size in pixels to 0 start with
	m_WindowSize.x = 0;               
	m_WindowSize.y = 0;

	// Initial pixel size, we're assuming that pixels are square
	// so this is both the x & y dimensions of a pixel in mm
	m_UnitsPerPixel = 0.25;

	// No gauges by default
	m_NumGauges = 0;

	// We should setup the GL stuff the first time we try to render
	m_DisplayIsInitialized = false;

	// NOT initially ok to render, since a rendering context probably
	// isn't around
	m_IsOKToRender = false;
}

RenderWindow::~RenderWindow()
{
	std::list<Gauge*>::iterator it;

	// If there are gauges, delete them
	if ( !m_NumGauges==0 )
	{    
		for (it = m_GaugeList.begin(); it != m_GaugeList.end(); ++it)
		{
			delete (*it);
		}
	}

}

void RenderWindow::SetupDisplay()
{
	// Clear The Background Color To Black
	glClearColor(0, 0, 0, 0);

	// Verify that we're drawing filled polys by default
	glPolygonMode(GL_FRONT, GL_FILL);
	
	// Enable vertex arrays
	glEnableClientState(GL_VERTEX_ARRAY);
	
	// Disable depth test (for better antialiasing)
	glDisable(GL_DEPTH_TEST);

	// Set line anti-aliasing
#if 0
	// No anti-aliasing
	glDisable(GL_BLEND);
	glDisable(GL_POINT_SMOOTH);
	glDisable(GL_LINE_SMOOTH);
	glDisable(GL_POLYGON_SMOOTH);
#else
	// Enable blending
	glEnable(GL_BLEND);

	// Select type Of blending
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	// Enable line and polygon smoothing
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glDisable(GL_POLYGON_SMOOTH); // seems to cause problems on Linux

	// We want the prettiest smooth lines possible
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
#endif
}

void RenderWindow::Resize(int cx, int cy)
{
	if(cx==0) // prevent div/0 (or it won't display)
		cx=1;
	if(cy==0) // prevent div/0
		cy=1;

	m_WindowSize.x = cx;
	m_WindowSize.y = cy;

	// create the viewport with the resized dimensions
	glViewport(0,0,cx,cy);

	// now change the clipping volume parameters
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	m_OrthoParams[0] = 0.0f; // left
	m_OrthoParams[1] = m_UnitsPerPixel * cx; // right
	m_OrthoParams[2] = 0.0f; // bottom
	m_OrthoParams[3] = m_UnitsPerPixel *cy; //top
	m_OrthoParams[4] = 1.0f; //near clipping
	m_OrthoParams[5] = -1.0f; //far clipping

	glOrtho(m_OrthoParams[0], m_OrthoParams[1], m_OrthoParams[2],
			m_OrthoParams[3], m_OrthoParams[4], m_OrthoParams[5]);

	glMatrixMode(GL_MODELVIEW);
}

void RenderWindow::Render()
{
	// First see if we're allowed to be doing this
	if(!m_IsOKToRender)
		return;

	// Next see if the display is set up
	if (!m_DisplayIsInitialized)
	{
		this->SetupDisplay();
		m_DisplayIsInitialized = true;
	}

	// Clear the frame buffer
	glClear(GL_COLOR_BUFFER_BIT);

	// load the modelview matrix
	glMatrixMode(GL_MODELVIEW);

	// reset the modelview matrix
	glLoadIdentity();

	// Draw all of the gauges
	this->RenderGauges();
}

void RenderWindow::RenderGauges()
{
	std::list<Gauge*>::iterator it;

	// Walk the list of gauges and render them
	for (it = m_GaugeList.begin(); it != m_GaugeList.end(); ++it)
	{
		(*it)->Render();
	}
}

void RenderWindow::AddGauge(Gauge* pGauge)
{
	m_GaugeList.push_back(pGauge);
	m_NumGauges++;
}

#if 0
GLboolean RenderWindow::CheckExtension( char *extName )
{
	/*
	 ** Search for extName in the extensions string.  Use of strstr()
	 ** is not sufficient because extension names can be prefixes of
	 ** other extension names.  Could use strtok() but the constant
	 ** string returned by glGetString can be in read-only memory.
	 */
	char *p = (char *) glGetString(GL_EXTENSIONS);
	char *end;
	int extNameLen;

	extNameLen = strlen(extName);
	end = p + strlen(p);

	while (p < end)
	{
		int n = strcspn(p, " ");
		if ((extNameLen == n) && (strncmp(extName, p, n) == 0))
		{
			return GL_TRUE;
		}
		p += (n + 1);
	}
	return GL_FALSE;
}
#endif

void RenderWindow::CallBackKeyboardFunc(int keycode, int modifiers)
{
	std::list<Gauge*>::iterator it;

	switch(keycode)
	{
		case '1':
			{
				sleep(2); // freeze OpenGC for 2 second
			}
			break;
		case 'q':
			{
				if (modifiers & 0x04)
				{
					globals->m_MessageableList->DispatchMessage(MSG_APP_QUIT, NULL);
					break;
				}
			}
		default:
			{
				printf("Keyboard event %i \"%c\"\n", keycode, keycode);

				// Its not app-global, so give the event to each gauge
				for (it = m_GaugeList.begin(); it != m_GaugeList.end(); ++it)
					(*it)->OnKeyboard(keycode, modifiers);
			}
			break;
	}
}

void RenderWindow::CallBackMouseFunc(int button, int state, int x, int y)
{
	// Since the coordinate system origin for the window is in the upper left
	// and that of our gauges is in the lower left, we need to invert y
	y = m_WindowSize.y - y;

	std::list<Gauge*>::iterator it;

	// Walk the list of gauges and apply the mouse click
	for (it = m_GaugeList.begin(); it != m_GaugeList.end(); ++it)
		(*it)->HandleMouseButton(button, state, x, y);
}

} // end namespace OpenGC
