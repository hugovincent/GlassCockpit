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

//--------Base-----------------
#include "GLHeaders.h"
#include <FL/Fl.H>
#include "AppObject.h"
#include "RenderObject.h"
#include "Globals.h"
#include "Debug.h"

//--------Data Sources---------
#include "AlbatrossDataSource.h"
#include "TestDataSource.h"
#include "FGDataSource.h"

//--------Gauges---------------
#include "PFD.h"
#include "NavDisplay.h"
#include "EngineInstruments.h"

namespace OpenGC
{

AppObject::AppObject()
{
	// Make sure all the pointers are nulled out
	m_pRenderWindow = 0;
	m_CalcManager = 0;
}

AppObject::~AppObject()
{
	if(m_pRenderWindow != 0)
	{
		delete m_pRenderWindow;
		m_pRenderWindow = 0;
	}

	if (m_CalcManager != 0)
	{
		delete m_CalcManager;
		m_CalcManager = 0;
	}
}

bool AppObject::Go(XMLNode rootNode)
{	
	// Navigation Database (global)
	globals->m_NavDatabase->InitDatabase();

	// Create the data source
	XMLNode dsNode = rootNode.GetChild("DataSource");
	Assert(dsNode.HasProperty("type"), "DataSource node has no type property");
	string dsName = dsNode.GetProperty("type");
	char *titleSuffix;
	if (dsName == "FlightGear")
	{
		// Get host/port settings
		if (dsNode.HasChild("Host"))
		{
			globals->m_PrefManager->SetPrefS("FlightGearHost", 
					dsNode.GetChild("Host").GetText());
		}
		if (dsNode.HasChild("Port"))
		{
			globals->m_PrefManager->SetPrefI("FlightGearPort", 
					dsNode.GetChild("Port").GetTextAsInt());
		}

		globals->m_DataSource = new FGDataSource();
		if (!globals->m_DataSource->Open())
		{
			return false;
		}
		titleSuffix = (char *)" (FlightGear)";
	}
	else if (dsName == "Albatross")
	{
		globals->m_DataSource = new AlbatrossDataSource();
		titleSuffix = (char *)" (Flight Mode)";
	}
	else if (dsName == "Test")
	{
		globals->m_DataSource = new TestDataSource();
		titleSuffix = (char *)" (Test)";
	}
	else
	{
		printf("Invalid data source \"%s\".\n", dsName.c_str());
		return false;
	}

	// Create the data calculations manager (CalcManager)
	m_CalcManager = new CalcManager();
	m_CalcManager->InitFromXMLNode(XMLNode() /*FIXME*/);

	// Set up the window title
	XMLNode windowNode = rootNode.GetChild("Window");
	string windowTitle = "Glass Cockpit";
	if (windowNode.HasChild("Title"))
	{
		windowTitle = windowNode.GetChild("Title").GetText();
	}
	windowTitle += titleSuffix;
	
	// Calculate window size
	double xSize = 1127.0, ySize = 785.0; // default size
	if (windowNode.HasChild("Geometry"))
	{
		XMLNode geoNode = windowNode.GetChild("Geometry");
		if (geoNode.HasChild("Size"))
		{
			geoNode.GetChild("Size").GetTextAsCoord(xSize, ySize);
		}
	}
	double zoom = globals->m_PrefManager->GetPrefD("Zoom");
	int windowX = (int)(xSize * zoom), windowY = (int)(ySize * zoom);
	LogPrintf("Application: Window Size = %ix%ipx\n", windowX, windowY);

	// Create the render window
	m_pRenderWindow = new FLTKRenderWindow(4, 0, windowX, windowY, 
			windowTitle.c_str());
	m_pRenderWindow->mode(FL_RGB | FL_DOUBLE);
	globals->m_PrefManager->SetPrefD("UnitsPerPixel", 
			m_pRenderWindow->GetUnitsPerPixel());	

	// We need to go ahead and show the window so that an OpenGL device
	// context exists once we start loading fonts
	m_pRenderWindow->show();

	// Force the render window to update to obtain a valid device context
	m_pRenderWindow->redraw();
	Fl::flush();

	// Create Gauges as described by the XML file
	XMLNode::NodeList nodeList = windowNode.GetChildList("Gauge");
	XMLNode::NodeList::iterator iter;
	for (iter = nodeList.begin(); iter != nodeList.end(); ++iter)
	{
		Gauge *pGauge;
		string name = (*iter).GetProperty("type");

		if(name == "PFD")
		{
			pGauge = new PFD();
		}
		else if(name == "NavDisplay")
		{
			pGauge = new NavDisplay();
		}
		else if(name == "EngineInstruments")
		{
			pGauge = new EngineInstruments();
		}
		else
		{
			printf("Error: unsupported gauge type \"%s\"\n", name.c_str());
			return false;
		}

		pGauge->InitFromXMLNode(*iter);
		m_pRenderWindow->AddGauge(pGauge);
	}

	// Now it's time to enter the event loop
	Fl::run();

	return true; // sucess
}

void AppObject::IdleFunction()
{
	// Every time we loop we grab some new data...
	bool changed1 = globals->m_DataSource->OnIdle();

	// ...calculate extra data from the incoming data...
	bool changed2 = m_CalcManager->Calculate();

	// and re-render the window if there is new data.
	if(changed1 || changed2)
	{
		m_pRenderWindow->redraw();
		Fl::flush();
	}
}

void AppObject::OnMessage(Message message, void *data)
{

}

} // end namespace OpenGC

