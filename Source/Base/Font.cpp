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

#include "GLHeaders.h"
#include <stdio.h>

#include "Font.h"
#include "Debug.h"

namespace OpenGC
{

Font::Font()
{
	// Default is a unit sized font
	m_Size.x = 1.0;
	m_Size.y = 1.0;
	m_Spacing = 1.0; // FIXME spacing doesn't get used at all

	m_PolygonFont = 0;
	m_OutlineFont = 0;
	m_RightAligned = false;

	m_Name = 0;
}

Font::~Font()
{
	if(m_PolygonFont != 0)
	{
		delete m_PolygonFont;
	}
	m_PolygonFont = 0;

	if(m_OutlineFont != 0)
	{
		delete m_OutlineFont;
	}
	m_OutlineFont = 0;
}

void Font::SetSize(double x, double y)
{
	m_Size.x = x;
	m_Size.y = y;
}

void Font::Print(double x, double y, const char *string)
{
	Check(m_PolygonFont != NULL);
	Check(m_OutlineFont != NULL);
	
	// Save the modelview matrix
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	// Set position and size
	glTranslated(x,y,0);
	glScaled(0.0135*m_Size.x, 0.0135*m_Size.y, 1);

	if (m_RightAligned)
	{
		// We offset by the width of the text, so the right hand edge
		// of the string is rendered at the provided coordinates.
		glTranslated(-1.0 * m_PolygonFont->Advance(string), 0, 0);
	}
	
	// Draw using the triangulated font
	glPushMatrix(); // this is a hack to fix anti-aliasing
	m_PolygonFont->Render(string);
	glPopMatrix(); // this is a hack to fix anti-aliasing

	// Draw the outline font for smoothing
	glLineWidth(1.0);
	m_OutlineFont->Render(string);

	// Restore modelview matrix
	glPopMatrix();
}

bool Font::LoadFont(char* name)
{
	m_Name = name;

	// Open the the font in both polygon and outline mode
	m_PolygonFont = new FTGLPolygonFont(name);
	m_OutlineFont = new FTGLOutlineFont(name);

	// The initial face size is large so that font sizing
	// will work correctly later on
	m_PolygonFont->FaceSize(100);
	m_OutlineFont->FaceSize(100);

	// We succeeded at opening the fonts
	return true;
}

} // end namespace OpenGC
