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
 * Render windows are the high-level window management object in OpenGC.
 * Although this class does not contain any virtual functions, it assumes
 * that a proper rendering context exists PRIOR to any of the GL code
 * executing. Creating a device context is left as an excercise for the
 * reader.
 *
 * NOTE that bool m_IsOKToRender is set to false in the constructor of
 * this class and essentially disables rendering until enabled in a
 * derived class (to help prevent accidental OpenGL shenanigans).
 */

#ifndef RenderWindow_h
#define RenderWindow_h

#include <list>
#include "OrderedPair.h"
#include "Gauge.h"

namespace OpenGC
{

class RenderWindow
{
	public:
		/** Constructor is responsible for initing a few variables */
		RenderWindow();

		~RenderWindow();

		/** Call to reset projection on resize. */
		void Resize(int x, int y);

		/** Initialize GL parameters. */
		void SetupDisplay();
	
		void ForceIsOKToRender(bool newVal) { m_IsOKToRender = newVal; }

		/** Render the entire window. */
		void Render();

		/** Add a gauge to the window */
		void AddGauge(Gauge* pGauge);

		/** Iterates through the gauges and invokes the render method */
		void RenderGauges();

		/** Check to see if an extension exists */
		GLboolean CheckExtension( char *extName );

		/** Keyboard callback
		 * keycode: ASCII code where available, else undefined
		 * modifiers: 0x01 =  shift, 0x04 = ctrl, 0x08 = alt, 0x40 = meta
		 */
		void CallBackKeyboardFunc(int keycode, int modifiers);

		/**  Mouse callback
		 * button: 1 = left, 2 = middle, 3 = right
		 * state: 0 = down, 1 = up FIXME drag/move
		 */
		void CallBackMouseFunc(int button, int state, int x, int y);

		/** Returns the mm/pixel for this render window */
		double GetUnitsPerPixel() { return m_UnitsPerPixel; }

	protected:

		/** Window height and width in pixels */
		OrderedPair<unsigned int> m_WindowSize;

		/**
		 * Physical size of a pixel (assumed to be square) in mm
		 * for the purpose of drawing gauges in approximately
		 * the correct physical size on the monitor. It's also correct to
		 * refer to this as the dot pitch of the monitor in mm.
		 */
		double m_UnitsPerPixel;

		/** Parameters for the orthographic projection */
		float m_OrthoParams[6];

		/** All of the guages */
		std::list<Gauge*> m_GaugeList;

		/** The number of gauges used in the display */
		int m_NumGauges;

		/** True if the render window has been initialized */
		bool m_DisplayIsInitialized;

		/** True if it's safe to render things */
		bool m_IsOKToRender;
};

} // end namespace OpenGC

#endif

