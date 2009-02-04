/*=========================================================================

  OpenGC - The Open Source Glass Cockpit Project
  Please see our web site at http://www.opengc.org

  Albatross UAV Project - http://www.albatross-uav.org

  Copyright (c) 2006 Hugo Vincent.
  All rights reserved.

  This file is distributed under the terms of the GNU General Public
  License version 2.0. See included COPYING file for details.

 =========================================================================*/

#include "Globals.h"
#include "GLHeaders.h"
#include "PFDOverlay.h"

namespace OpenGC
{

PFDOverlay::PFDOverlay()
{
	m_Font = globals->m_FontManager->LoadDefaultFont();
	this->SetOpaque(false); // this is overlaid so we don't want an opaque background
	
	m_PhysicalSize.x = 200;
	m_PhysicalSize.y = 190;

	m_Scale.x = 1.0;
	m_Scale.y = 1.0;
}

PFDOverlay::~PFDOverlay()
{

}
	
void PFDOverlay::Render()
{
	GaugeComponent::Render();
		
	AirframeDataContainer *data = globals->m_DataSource->GetAirframe();
	
	char buffer[16];

	// Draw heading indicator track as text
	glColor3d(255, 255, 255); // white
	sprintf(buffer, "%3d", (int) data->GetTrue_Heading());
	globals->m_FontManager->SetSize(m_Font, 4.0, 4.0 );
	globals->m_FontManager->Print(75, 7, "GPS", m_Font); 
	globals->m_FontManager->SetRightAligned(m_Font, true);
	globals->m_FontManager->Print(100, 7, &buffer[0], m_Font); 
	globals->m_FontManager->SetRightAligned(m_Font, false);
}

} // end namespace OpenGC
