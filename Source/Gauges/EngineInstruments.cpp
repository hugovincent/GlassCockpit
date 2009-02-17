/*=========================================================================

  OpenGC - The Open Source Glass Cockpit Project
  Please see our web site at http://www.opengc.org

  Albatross UAV Project - http://www.albatross-uav.org

  Copyright (c) 2006 Hugo Vincent
  All rights reserved.
  See Copyright.txt or http://www.opengc.org/Copyright.htm for details.

  This software is distributed WITHOUT ANY WARRANTY; without even 
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
  PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include <stdio.h>
#include <time.h>
#include <sys/time.h>

#include "Globals.h"
#include "Constants.h"
#include "DataSource.h"
#include "EngineInstruments.h"
#include "PieDial.h"
#include "MarkedDial.h"
#include "GenericBargraph.h"
#include "Tachometer.h"

namespace OpenGC
{

EngineInstruments::EngineInstruments()
{
	m_PhysicalSize.x = 195;
	m_PhysicalSize.y = 80;

	m_Scale.x = 1.0;
	m_Scale.y = 1.0;

	// We need a font to draw the text
	m_Font = globals->m_FontManager->LoadDefaultFont();

	// Tachometer
	Tachometer* pTacho = new Tachometer();
	pTacho->SetParentRenderObject(this);
	pTacho->SetPosition(2,35);
	this->AddGaugeComponent(pTacho);

	// PieDial gauge for EGT
	PieDial* pEGT = new PieDial();
	pEGT->SetParentRenderObject(this);
	pEGT->SetPosition(48,35);
	pEGT->SetDataSource(&AirframeDataContainer::GetEngine_EGT);
	pEGT->SetMinMax(0.0, 1000.0);
	pEGT->SetColourRanges(850.0, 925.0);
	this->AddGaugeComponent(pEGT);

	// PieDial gauge for CHT
	PieDial* pCHT = new PieDial();
	pCHT->SetParentRenderObject(this);
	pCHT->SetPosition(96,35);
	pCHT->SetDataSource(&AirframeDataContainer::GetEngine_CHT);
	pCHT->SetMinMax(0.0, 250.0);
	pCHT->SetColourRanges(180.0, 230.0);
	this->AddGaugeComponent(pCHT);

#if 1
	// PieDial gauge for FIXME
	PieDial* pFIXME = new PieDial();
	pFIXME->SetParentRenderObject(this);
	pFIXME->SetPosition(2,0);
	pFIXME->SetDataSource(&AirframeDataContainer::GetEngine_CHT);
	pFIXME->SetMinMax(0.0, 250.0);
	pFIXME->SetColourRanges(180.0, 230.0);
	this->AddGaugeComponent(pFIXME);
	
	// PieDial gauge for FIXME 2
	PieDial* pFIXME2 = new PieDial();
	pFIXME2->SetParentRenderObject(this);
	pFIXME2->SetPosition(48,0);
	pFIXME2->SetDataSource(&AirframeDataContainer::GetEngine_EGT);
	pFIXME2->SetMinMax(0.0, 1000.0);
	pFIXME2->SetColourRanges(850.0, 925.0);
	this->AddGaugeComponent(pFIXME2);
	
	// PieDial gauge for FIXME 3
	PieDial* pFIXME3 = new PieDial();
	pFIXME3->SetParentRenderObject(this);
	pFIXME3->SetPosition(96,0);
	pFIXME3->SetDataSource(&AirframeDataContainer::GetEngine_CHT);
	pFIXME3->SetMinMax(0.0, 250.0);
	pFIXME3->SetColourRanges(140.0, 185.0);
	this->AddGaugeComponent(pFIXME3);
#endif
	
	// Create a MarkedDial gauge FIXME
	MarkedDial* pDial = new MarkedDial();
	pDial->SetParentRenderObject(this);
	pDial->SetPosition(150,3);
	pDial->SetDataSource(&AirframeDataContainer::GetEngine_Mixture);
	pDial->SetMinMax(0.0, 9.0);
	pDial->SetTickSpacing(2.0);
	pDial->SetTickDivisor(1.0);
	this->AddGaugeComponent(pDial);

	// Bargraph voltage displays for battery and generator
	GenericBargraph* pBattVoltage = new GenericBargraph();
	pBattVoltage->SetParentRenderObject(this);
	pBattVoltage->SetPosition(150,38);
	pBattVoltage->SetDataSource(&AirframeDataContainer::GetEngine_Mixture);
	pBattVoltage->SetMinMax(0.0, 16.0);
	pBattVoltage->SetColourRanges(11.0, 9.0);
	this->AddGaugeComponent(pBattVoltage);

	GenericBargraph* pGenVoltage = new GenericBargraph();
	pGenVoltage->SetParentRenderObject(this);
	pGenVoltage->SetPosition(170,38);
	pGenVoltage->SetDataSource(&AirframeDataContainer::GetEngine_Mixture);
	pGenVoltage->SetMinMax(0.0, 16.0);
	pGenVoltage->SetColourRanges(11.0, 9.0);
	this->AddGaugeComponent(pGenVoltage);
}

EngineInstruments::~EngineInstruments()
{
	// Destruction handled by base class
}

void EngineInstruments::Render()
{
	Gauge::Render();

	glColor3ub(0,190,190); // cyan

	// We want to draw the divider between the engine instruments and the PFD/Nav above
   	glLineWidth( 2.0 );
   	glColor3ub( 0, 190, 190 ); // cyan
   	glBegin(GL_LINES);
   	glVertex2f(0.0, m_PhysicalSize.y);
   	glVertex2f(m_PhysicalSize.x, m_PhysicalSize.y);
   	glEnd();

	// Draw the text labels
	globals->m_FontManager->SetSize(m_Font, 5, 6);
	globals->m_FontManager->Print(15, 70, "RPM", m_Font);
	globals->m_FontManager->Print(66, 70, "EGT", m_Font);
	globals->m_FontManager->Print(111, 70, "CHT", m_Font);

	// Draw frames per second
	char buffer[10];
	sprintf(buffer, "FPS  %.1f", GetFPS());
	globals->m_FontManager->SetSize(m_Font, 3, 3);
	globals->m_FontManager->Print(169, 2, buffer, m_Font);
}

/** Called once per frame, returns estimated frames per second. */
double EngineInstruments::GetFPS()
{
	static double rate, last;
	static int count;
	static int updateRate = (int)(1.0 / globals->m_PrefManager->GetPrefD(
				"AppUpdateRate"));

	double now;
	struct timeval tv;
	struct timezone tz;
	
	if (++count > updateRate)
	{ // recalculate roughly once per second
		gettimeofday(&tv, &tz);
		now = (double)tv.tv_usec/1e6 + (double)tv.tv_sec;
		rate = (double)updateRate / (now - last);
		last = now;
		count = 0;
	}

	return rate;
}

} // end namespace OpenGC

