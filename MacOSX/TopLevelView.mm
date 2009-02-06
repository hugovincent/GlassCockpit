//
//  TopLevelView.m
//  GlassCockpit
//
//  Created by Hugo Vincent on 4/02/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "TopLevelView.h"
#include <OpenGL/gl.h>

//--------Base-----------------
#include "Debug.h"
#include "Globals.h"
#include "XMLParser.h"

//--------Data Sources---------
#include "AlbatrossDataSource.h"
#include "TestDataSource.h"
#include "FGDataSource.h"

//--------Gauges---------------
#include "PFD.h"
#include "NavDisplay.h"
#include "EngineInstruments.h"

#define PATH_PREFIX				"/Users/hugo/Projects/iPhone/GlassCockpit/"
#define DEFAULT_XML_FILE		PATH_PREFIX "Data/Default.xml"
#define PREFERENCES_XML_FILE	PATH_PREFIX "Data/Preferences.xml"

using namespace OpenGC;

// Create the Globals object
Globals *OpenGC::globals;

@interface TopLevelView (PrivateMethods)
- (BOOL)go:(XMLNode*)rootNode;
- (void)IdleFunction;
@end

@implementation TopLevelView

- (void)awakeFromNib {

	// Construct the application
	OpenGC::globals = new Globals();
	
	// Initialise preferences manager
	globals->m_PrefManager->InitPreferences(PREFERENCES_XML_FILE);
	
	// Read the XML file and do some basic checks about its contents
	XMLParser parser;
	Assert(parser.ReadFile(DEFAULT_XML_FILE), "unable to read XML file");
	Check(parser.HasNode("/"));
	Assert(parser.HasNode("/Window"), "invalid XML, no Window node");
	Assert(parser.HasNode("/DataSource"), "invalid XML, no DataSource node");
	
	// Set the user-defined (in XML file) application preferences
	if (parser.HasNode("/Preferences"))
	{
		globals->m_PrefManager->SetPrefsFromXML(parser.GetNode("/Preferences"));
	}
	
	// FIXME debug:
	globals->m_PrefManager->PrintAll();
	
	// Run up the application
	XMLNode rootNode = parser.GetNode("/");
	size = NSMakeSize(0, 0);
	[self go:&rootNode];
}


- (void)dealloc {

	delete m_pRenderWindow;
	delete m_CalcManager;
	delete globals;
	
	[super dealloc];
}


- (void)reshape {

	if ([self frame].size.width != size.width || [self frame].size.height != size.height) {
		
		m_pRenderWindow->Resize(size.width, size.height);	
		size = [self frame].size;
	}
	[self setNeedsDisplay:YES];
}


- (void)drawRect:(NSRect)rect {
	
	NSOpenGLContext *ctx = [self openGLContext];
//	[ctx makeCurrentContext];
	m_pRenderWindow->Render();
	[ctx flushBuffer];
}

- (BOOL)go:(XMLNode*)rootNode {
	
	// Font manager (global)
	globals->m_FontManager->SetFontPath(globals->m_PrefManager->GetPrefS("FontPath").c_str());
	
	// Navigation Database (global)
	globals->m_NavDatabase->InitDatabase(globals->m_PrefManager->GetPrefS("NavPath").c_str());
	
	// Create the data source
	XMLNode dsNode = rootNode->GetChild("DataSource");
	Assert(dsNode.HasProperty("type"), "DataSource node has no type property");
	string dsName = dsNode.GetProperty("type");
	char *titleSuffix;
	if (dsName == "FlightGear")
	{
		// Get host/port settings
		if (dsNode.HasChild("Host"))
		{
			printf("here\n"); fflush(stdout);
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
		titleSuffix = " (FlightGear)";
	}
	else if (dsName == "Albatross")
	{
		globals->m_DataSource = new AlbatrossDataSource();
		titleSuffix = " (Flight Mode)";
	}
	else if (dsName == "Test")
	{
		globals->m_DataSource = new TestDataSource();
		titleSuffix = " (Test)";
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
	XMLNode windowNode = rootNode->GetChild("Window");
	string windowTitle = "Albatross Glass Cockpit";
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
	m_pRenderWindow = new RenderWindow(); //4, 0, windowX, windowY, windowTitle.c_str());
	//	m_pRenderWindow->mode(FL_RGB | FL_DOUBLE); // FIXME
	globals->m_PrefManager->SetPrefD("UnitsPerPixel", m_pRenderWindow->GetUnitsPerPixel());
	
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
	
	m_pRenderWindow->ForceIsOKToRender(true);
	// Set the update rate in nominal seconds per frame	
	animationInterval = globals->m_PrefManager->GetPrefD("AppUpdateRate");
	animationTimer = [NSTimer scheduledTimerWithTimeInterval:animationInterval target:self selector:@selector(IdleFunction) userInfo:nil repeats:YES];
	
	return YES; // sucess	
}

- (void)IdleFunction {
	
	// Every time we loop we grab some new data...
	bool changed1 = globals->m_DataSource->OnIdle();
	
	// ...calculate extra data from the incoming data...
	bool changed2 = m_CalcManager->Calculate();
	
	// and re-render the window if there is new data.
	if(changed1 || changed2)
	{
		[self setNeedsDisplay:YES];
	}
}

@end

#if 0	
	int AGLRenderWindow::handle(int event)
	{
		/*	//FIXME
		 switch(event) {
		 case FL_PUSH:		// Mouse down
		 {
		 this->CallBackMouseFunc(Fl::event_button(), 0,
		 Fl::event_x(), Fl::event_y() );
		 return 1;
		 }
		 case FL_DRAG:		// Mouse drag
		 {
		 return 1;
		 }
		 case FL_RELEASE:	// Mouse up
		 {
		 this->CallBackMouseFunc(Fl::event_button(), 1, 
		 Fl::event_x(), Fl::event_y() );
		 return 1;
		 }
		 case FL_FOCUS :
		 case FL_UNFOCUS :
		 {
		 // return 1 if you want keyboard events, 0 otherwise
		 return 1;
		 }
		 case FL_KEYDOWN:	// Keyboard key pressed
		 {
		 int key = Fl::event_key();
		 if (key < 128 && key >= 32)
		 this->CallBackKeyboardFunc(key, Fl::event_state() >> 16);
		 return 1;
		 }
		 default:
		 {
		 // pass other events to the base class, e.g. Alt+F4
		 return Fl_Gl_Window::handle(event);
		 }
		 }
		 */
	}
	
} // end namespace OpenGC

#endif
