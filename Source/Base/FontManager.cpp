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

#include <stdio.h>
#include <string.h>
#include "Font.h"
#include "FontManager.h"

namespace OpenGC
{

FontManager::FontManager()
{
	m_NumFonts = 0;

	// Allocate space for the font paths
	m_NameWithPath = new char[512];
	m_FontPath = new char[128];

	// The default font path for all platforms is "/", which is most likely wrong
	strcpy( m_FontPath, "/" );
}

FontManager::~FontManager()
{
	delete[] m_NameWithPath;
	delete[] m_FontPath;

	// Delete each font in the list
	std::vector<Font*>::iterator iter;
	for (iter = m_FontList.begin(); iter != m_FontList.end(); ++iter)
	{
		delete *iter;
	}
}

void FontManager::SetFontPath(const char* fontPath)
{
	strcpy(m_FontPath, fontPath);
}

/** Loads the default font, i.e. Bitstream Vera */
int FontManager::LoadDefaultFont()
{
	return this->LoadFont("bitstream_vera.ttf");
}

int FontManager::LoadFont(const char* name)
{
	// Concatenate the font name onto the font path
	m_NameWithPath = strcpy(m_NameWithPath, m_FontPath);
	strcat(m_NameWithPath, name);

	// First, check to see if the font is already loaded
	for(int i = 0; i < m_NumFonts; i++)
	{
		if (m_NameWithPath == m_FontList[i]->GetName() )
		{
			return i;
		}
	}

	// Ok, the font isn't loaded, so create a new one
	Font* pFont = new Font;

	if( pFont->LoadFont(m_NameWithPath) )
	{
		// Add the font to the list
		m_FontList.push_back(pFont);

		// Update the number of stored fonts
		m_NumFonts++;

		// Return the index of the new font
		return (m_NumFonts - 1);
	}
	else
	{
		delete pFont;
		// Return the flag that means we failed
		return -1;
	}
}

void FontManager::SetSize(int font, double x, double y)
{
	if( (font >= 0) && (font < m_NumFonts) )
	{
		m_FontList[font]->SetSize(x,y);
	}
}

void FontManager::SetRightAligned(int font, bool rightAligned)
{
	if( (font >= 0) && (font < m_NumFonts) )
	{
		m_FontList[font]->SetRightAligned(rightAligned);
	}
}

void FontManager::SetSpacing(int font, double spacing)
{
	if( (font >= 0) && (font < m_NumFonts) )
	{
		m_FontList[font]->SetSpacing(spacing);
	}
}

void FontManager::Print(double x, double y, const char* text, int font)
{
	if( (font >= 0) && (font < m_NumFonts) )
	{
		m_FontList[font]->Print(x,y,text);
	}
}

} // end namespace OpenGC
