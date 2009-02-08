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
 * FontManager allows sharing of fonts between different gauge
 * components; this is important so that memory-intensive fonts
 * are limited to a single instance in memory. If a gauge component
 * requests a font which is already present in memory, the font
 * is _not_ loaded again.
 *
 * Fonts are stored in a STL vector.
 *
 * Note: only one FontManager should be used in an application!!
 * Nothing bad will happen if more than one is used, but it defeats
 * the purpose using it in the first place (i.e. memory conservation)
 */

#ifndef FontMangager_h
#define FontMangager_h

#include <vector>
#include <string>
#include "Font.h"

namespace OpenGC
{
	
using namespace std;

class FontManager  
{
	public:
		FontManager();
		~FontManager();

		/**
		 * Request that we load the default gauge font
		 * This is nice because we can globally change the default and
		 * affect all of the gagues
		 */
		int LoadDefaultFont();

		/**
		 * Request that a new font be loaded; returns the index of the font
		 * in the font list (vector)
		 */
		int LoadFont(const string& name);

		/** Set the size of a particular font in physical units */
		void SetSize(int font, double x, double y);

		/** Set whether to render a font right-aligned */
		void SetRightAligned(int font, bool rightAligned);

		/** Print a character string at location (x,y) using specified font */
		void Print(double x, double y, const char* text, int font);

	protected:

		/** Path to Truetype fonts */
		static string m_FontPath;

		/**
		 * String used to hold the value of the font name with full path
		 */
		string m_NameWithPath;

		/** How many fonts are loaded */
		int m_NumFonts;

		/**
		 * The vector that holds Font objects
		 * We use a vector rather than a true linked list to allow random access
		 * to fonts, rather than having to traverse the entire list each time
		 */
		std::vector<Font*> m_FontList;
};

} // end namespace OpenGC

#endif // !defined(FontMangager_h)
