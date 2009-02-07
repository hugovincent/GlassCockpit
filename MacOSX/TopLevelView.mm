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
#include "BinaryNavData.h"

//--------Data Sources---------
#include "AlbatrossDataSource.h"
#include "TestDataSource.h"
#include "FGDataSource.h"

//--------Gauges---------------
#include "PFD.h"
#include "NavDisplay.h"
#include "EngineInstruments.h"

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
	NSString *prefsFileName = [[NSBundle mainBundle] pathForResource:@"Preferences" ofType:@"xml"];
	globals->m_PrefManager->InitPreferences([prefsFileName cStringUsingEncoding:NSASCIIStringEncoding]);
	
	// Read the XML file and do some basic checks about its contents
	XMLParser parser;
	NSString *xmlFileName = [[NSBundle mainBundle] pathForResource:@"Default" ofType:@"xml"];
	Assert(parser.ReadFile([xmlFileName cStringUsingEncoding:NSASCIIStringEncoding]), "unable to read XML file");
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

	size = [self frame].size;
	m_pRenderWindow->Resize(size.width, size.height);
	[self setNeedsDisplay:YES];
}


- (void)drawRect:(NSRect)rect {
	
	if ([self frame].size.width != size.width || [self frame].size.height != size.height) {
		[self reshape];
	}
	NSOpenGLContext *ctx = [self openGLContext];
	[ctx makeCurrentContext];
	m_pRenderWindow->Render();
	[ctx flushBuffer];
}

- (BOOL)go:(XMLNode*)rootNode {
	
	const char* resourcePath = [[[[NSBundle mainBundle] resourcePath] stringByAppendingString:@"/"]
								cStringUsingEncoding:NSASCIIStringEncoding];
	
	// Font manager (global)
	globals->m_FontManager->SetFontPath(resourcePath);
	
	// Navigation Database (global)
	globals->m_NavDatabase->InitDatabase(resourcePath);
	
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


#pragma mark Event Handling

- (BOOL)acceptsFirstResponder {

    return YES;
}


- (void)mouseDown:(NSEvent *)theEvent {

    NSPoint location = [theEvent locationInWindow];
	location.y = [self frame].size.height -location.y; // hack to match FLTK
	int buttonNumber;
	switch ([theEvent buttonNumber]) {
		case NSLeftMouseDown:
			buttonNumber = 1; break;
		case NSOtherMouseDown:
			buttonNumber = 2; break;
		case NSRightMouseDown:
			buttonNumber = 3; break;
		default:
			[[self nextResponder] mouseUp:theEvent];
			
	}
	m_pRenderWindow->CallBackMouseFunc(buttonNumber, 0, location.x, location.y);
}


- (void)mouseUp:(NSEvent *)theEvent {
	
    NSPoint location = [theEvent locationInWindow];
	location.y = [self frame].size.height -location.y; // hack to match FLTK
	int buttonNumber;
	switch ([theEvent buttonNumber]) {
		case NSLeftMouseUp:
			buttonNumber = 1; break;
		case NSOtherMouseUp:
			buttonNumber = 2; break;
		case NSRightMouseUp:
			buttonNumber = 3; break;
		default:
			[[self nextResponder] mouseUp:theEvent];
			
	}
	m_pRenderWindow->CallBackMouseFunc(buttonNumber, 1, location.x, location.y);
}


- (void)keyDown:(NSEvent *)theEvent {
 
	NSString *str = [theEvent charactersIgnoringModifiers];
	int key = [str characterAtIndex:0];
	
	// modifiers: 0x01 =  shift, 0x04 = ctrl, 0x08 = alt, 0x40 = meta
	int modifiers = 0;
	if ([theEvent modifierFlags] & NSShiftKeyMask)
		modifiers |= 0x01;
	if ([theEvent modifierFlags] & NSControlKeyMask)
		modifiers |= 0x04;
	if ([theEvent modifierFlags] & NSAlternateKeyMask) // option - we map this as meta
		modifiers |= 0x40;
	if ([theEvent modifierFlags] & NSCommandKeyMask)
		modifiers |= 0x08;
	
	if (key < 128 && key >= 32)
		m_pRenderWindow->CallBackKeyboardFunc(key, modifiers);
	else
		[[self nextResponder] keyDown:theEvent];
}

@end
