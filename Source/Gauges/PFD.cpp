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

#include "PFD.h"

#include "AlbatrossAnnunciator.h"
#include "ArtificialHorizon.h"
#include "AltitudeTape.h"
#include "AltitudeTicker.h"
#include "EngineInstruments.h"
#include "FlightDirector.h"
#include "HeadingIndicator.h"
#include "PFDOverlay.h"
#include "SpeedTape.h"
#include "SpeedTicker.h"
#include "VSI.h"

namespace OpenGC
{

PFD::PFD()
{
	m_PhysicalSize.x = 200;
	m_PhysicalSize.y = 190;

	m_Scale.x = 1.0;
	m_Scale.y = 1.0;

	// Create a heading indicator
	HeadingIndicator* pHeadingIndicator = new HeadingIndicator();
	pHeadingIndicator->SetParentRenderObject(this);
	pHeadingIndicator->SetPosition(29,5);
	this->AddGaugeComponent(pHeadingIndicator);

	// Create an artficial horizon
	ArtificialHorizon* pHorizon = new ArtificialHorizon();
	pHorizon->SetParentRenderObject(this);
	pHorizon->SetPosition(42,52);
	this->AddGaugeComponent(pHorizon);

	// Create an altitude tape
	AltitudeTape* pAltTape = new AltitudeTape();
	pAltTape->SetParentRenderObject(this);
	pAltTape->SetPosition(149,32);
	this->AddGaugeComponent(pAltTape);

	// Create an altitude ticker
	AltitudeTicker* pAltTicker = new AltitudeTicker();
	pAltTicker->SetParentRenderObject(this);
	pAltTicker->SetPosition(156,90);
	this->AddGaugeComponent(pAltTicker);

	// Create a VSI
	VSI* pVSI = new VSI();
	pVSI->SetParentRenderObject(this);
	pVSI->SetPosition(181,44);
	this->AddGaugeComponent(pVSI);

	// Create a speed tape
	SpeedTape* pSpeedTape = new SpeedTape();
	pSpeedTape->SetParentRenderObject(this);
	pSpeedTape->SetPosition(8,32);
	this->AddGaugeComponent(pSpeedTape);

	// Create a speed ticker
	SpeedTicker* pSpeedTicker = new SpeedTicker();
	pSpeedTicker->SetParentRenderObject(this);
	pSpeedTicker->SetPosition(7,90);
	this->AddGaugeComponent(pSpeedTicker);

	// Create a Albatross Annunciator Panel
	AlbatrossAnnunciator* pAnnun = new AlbatrossAnnunciator();
	pAnnun->SetParentRenderObject(this);
	pAnnun->SetPosition(40,160);
	this->AddGaugeComponent(pAnnun);

	// Create the overlaid Flight Director
	FlightDirector* pFlightDir = new FlightDirector();
	pFlightDir->SetParentRenderObject(this);
	pFlightDir->SetPosition(0,0);
	this->AddGaugeComponent(pFlightDir);

	// Create the PFD Overlay
	PFDOverlay* pOverlay = new PFDOverlay();
	pOverlay->SetParentRenderObject(this);
	pOverlay->SetPosition(0,0);
	this->AddGaugeComponent(pOverlay);
}

PFD::~PFD()
{
	// Destruction handled by base class
}

void PFD::Render()
{
	Gauge::Render();
}

} // end namespace OpenGC

