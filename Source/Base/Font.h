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
 * Wraps the FTGL font toolkit to make its interface fit well into the
 * OpenGC environment. Font's are accessed via the FontManager object,
 * which all RenderObject's contain a pointer to (thereby eliminating
 * redundant font storage).

 * FTGL is capable of reading any True-Type font, allowing the generation of
 * very high quality font renderings
 */

#ifndef Font_h
#define Font_h

#include "OrderedPair.h"
#include <string>

// It's important to define this since we're linking against
// a static rather than shared version of FTGL
#define FTGL_LIBRARY_STATIC

#include <FTGL/ftgl.h>

namespace OpenGC
{
	
using namespace std;

class Font
{
	public:
		Font();
		~Font();

		/** Load a font from a specified file */
		bool LoadFont(const string& name);

		/** Print a character string at location (x,y) */
		void Print(double x, double y, const char *string);

		/**
		 * Set the spacing of the font in fractional character widths
		 * I.e. spacing = 0.5 means 1/2 character width
		 */
		void SetSpacing(double spacing) { m_Spacing = spacing; }

		/** Set the size of the font (width and height) in physical units */
		void SetSize(double x, double y);

		/** Sets the font to render to the left of the provided coordinates */
		void SetRightAligned(bool rightAligned) {m_RightAligned = rightAligned; }

		/** Get the font name */
		string& GetName() { return m_Name; }

	protected:

		/** The name of the font */
		string m_Name;
		
		/** Whether to render right-aligned or not */
		bool m_RightAligned;

		/** Spacing between characters in the font, in fractional character widths */
		double m_Spacing;

		/** The size of the font in physical units */
		OrderedPair<double> m_Size;

		/** The triangulated FTGL font object */
		FTGLPolygonFont* m_PolygonFont;

		/** The outline FTGL font object used for smoothing */
		FTGLOutlineFont* m_OutlineFont;
};

} // end namespace OpenGC

#endif // !defined(Font_h)
