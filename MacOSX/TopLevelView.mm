//
//  TopLevelView.m
//  GlassCockpit
//
//  Created by Hugo Vincent on 4/02/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "TopLevelView.h"
#include <OpenGL/gl.h>
#include <OpenGL/OpenGL.h> // for CGL stuff for full-screen on OS X

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

using namespace OpenGC;

// Create the Globals object
Globals *OpenGC::globals;

@interface TopLevelView (PrivateMethods)
- (BOOL)go:(XMLNode*)rootNode;
- (void)IdleFunction;
@end

@implementation TopLevelView

- (void)awakeFromNib {

	// Force default pixel format (for some reason, Interface Builder isn't doing this reliably)
	NSOpenGLPixelFormat *pf = [NSOpenGLView defaultPixelFormat];
	// FIXME, might want to force NSOpenGLPFAColorSize = 32, NSOpenGLPFAAlphaSize = 8
	[self setPixelFormat:pf];
	
	// Construct the application
	OpenGC::globals = new Globals();
	
	// Initialise preferences manager
	NSString *prefsFileName = [[NSBundle mainBundle] pathForResource:@"Preferences" ofType:@"xml"];
	globals->m_PrefManager->InitPreferences([prefsFileName cStringUsingEncoding:NSASCIIStringEncoding]);
	
	// Find paths: location of read-only app resources
	const char* resourcePath = [[[[NSBundle mainBundle] resourcePath] stringByAppendingString:@"/"]
								cStringUsingEncoding:NSASCIIStringEncoding];
	globals->m_PrefManager->SetPrefS("PathToData", resourcePath);
	
	// Set map cache path (map cache is read-only) // FIXME
	globals->m_RasterMapManager->SetCachePath(RasterMapManager::RMM_CACHE_MGMAPS, 
								"/Users/hugo/Projects/iPhone/GlassCockpit/Data/MGMapsCache", "GoogleTer");
	
	// Find paths: writeable directory to cache resources in
	NSArray *paths = NSSearchPathForDirectoriesInDomains(NSApplicationSupportDirectory, NSUserDomainMask, YES);
	Assert([paths count] == 1, "can't uniquely identify a writable path for cached resources");
	NSString *writeableDir = [(NSString*)[paths objectAtIndex:0] stringByAppendingString:@"/GlassCockpit/"];
	globals->m_PrefManager->SetPrefS("PathToCaches", [writeableDir cStringUsingEncoding:NSASCIIStringEncoding]);
	
	// Create writable directory if necessary
	NSFileManager *fm = [NSFileManager defaultManager];
	BOOL isDir = NO;
	if (![fm fileExistsAtPath:writeableDir isDirectory:&isDir] || isDir == NO) {
		
		[fm createDirectoryAtPath:writeableDir attributes:nil];
	}
	
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


- (void)fullScreen
{
	NSOpenGLContext* fullScreenContext;
	GLint oldSwapInterval;
	int onDisplay;
	
	// Take control of all the displays where we're about to go FullScreen.
	NSArray* displays = [NSScreen screens];
	CGDisplayErr error = CGCaptureAllDisplays();
	if (error != CGDisplayNoErr)
	{
		NSLog(@"Failed to capture all displays");
		return;
	}
	// turn off the animation timer -- dont want it firing for no reason
//	[self updateAnimationTimers:FALSE];
	
	for (onDisplay = 0; onDisplay < [displays count] & !error; onDisplay++)
	{
		// Pixel Format Attributes for the FullScreen NSOpenGLContext
		NSOpenGLPixelFormatAttribute attrs[] =
		{
			// Specify that we want a full-screen OpenGL context.
			NSOpenGLPFAFullScreen,
			
			// We may be on a multi-display system (and each screen may be driven by a different renderer),
			// so we need to specify which screen we want to take over.
			NSOpenGLPFAScreenMask, CGDisplayIDToOpenGLDisplayMask(
				(CGDirectDisplayID)[[[[displays objectAtIndex:onDisplay] deviceDescription] objectForKey:@"NSScreenNumber"] pointerValue]),
			
			// Attributes Common to FullScreen and non-FullScreen
			NSOpenGLPFADoubleBuffer,
			NSOpenGLPFAColorSize, 24,
			NSOpenGLPFAAlphaSize, 8,
			NSOpenGLPFADepthSize, 24,
			
			NSOpenGLPFANoRecovery,
			//NSOpenGLPFAAccelerated,
			0
		};
		
		// Create the FullScreen NSOpenGLContext with the attributes listed above.
		NSOpenGLPixelFormat* pixelFormat=[[NSOpenGLPixelFormat alloc] initWithAttributes:attrs];
		
		// Create an NSOpenGLContext with the FullScreen pixel format.  By specifying the non-FullScreen contexts as our "shareContexts",
		// we automatically inherit all of the textures, display lists, and other OpenGL objects it has defined.
		fullScreenContext = [[NSOpenGLContext alloc] initWithFormat:pixelFormat shareContext:nil]; //[self openGLContext]];
		[pixelFormat release];
		
		if(fullScreenContext == nil)
		{
			NSLog(@"Failed to create fullScreenContext");
			error = 1;
			break;
		}
		
		// Enter FullScreen mode and make our FullScreen context the active context for OpenGL commands.
		[fullScreenContext setFullScreen];
		[fullScreenContext makeCurrentContext];
		NSLog(@"Running full screen");
		
		// Save the current swap interval so we can restore it later, and then set the new swap interval to lock us to the display's refresh rate.
		CGLContextObj cglContext = CGLGetCurrentContext();
		CGLGetParameter(cglContext, kCGLCPSwapInterval, &oldSwapInterval);
		GLint newSwapInterval = 1;
		CGLSetParameter(cglContext, kCGLCPSwapInterval, &newSwapInterval);
	}
#if 0
	//Main Event Loop
	// Now that we've got the screen, we enter a loop in which we alternately process input events and computer and render the next frame
	// of our animation.  The shift here is from a model in which we passively receive events handed to us by the AppKit to one in which we
	// are actively driving event processing.
	[scene setFullscreen:YES];
	while([scene fullscreen]&&!error)
	{
		NSAutoreleasePool* pool=[[NSAutoreleasePool alloc] init];
		
		// Check for and process input events.
		NSEvent *event;
		while(event=[NSApp nextEventMatchingMask:NSAnyEventMask untilDate:[NSDate distantPast] inMode:NSDefaultRunLoopMode dequeue:YES])
		{
			switch([event type])
			{
				case NSLeftMouseDown:
					if([event modifierFlags]&NSControlKeyMask)
					{
						//fake the right mouse button... this is rather annoying...
						[scene rightMouseDown:event];
					}
					else
					{
						[scene mouseDown:event];
					}
					break;
				case NSLeftMouseUp:
					if([event modifierFlags]&NSControlKeyMask)
					{
						//fake the right mouse button... this is rather annoying...
						[scene rightMouseUp:event];
					}
					else
					{
						[scene mouseUp:event];
					}
					break;
				case NSLeftMouseDragged:
					if([event modifierFlags]&NSControlKeyMask)
					{
						//fake the right mouse button... this is rather annoying...
						[scene rightMouseDragged:event];
					}
					else
					{
						[scene mouseDragged:event];
					}
					break;
				case NSRightMouseDown:
					[scene rightMouseDown:event];
					break;
				case NSRightMouseUp:
					[scene rightMouseUp:event];
					break;
				case NSRightMouseDragged:
					[scene rightMouseDragged:event];
					break;
				case NSKeyDown:
					[scene keyDown:event];
					break;
				default:
					break;
			}
		}
		
		//find min origin, max size
		NSRect mainBounds=[displayData displays][0];
		//int onDisplay;
		for(onDisplay=1;onDisplay<[displays count]&!error;onDisplay++)
		{
			NSRect virtualBounds=[displayData displays][onDisplay];
			mainBounds=NSUnionRect(mainBounds,virtualBounds);
		}
		
		// Render a frame, and swap the front and back buffers.
		//Render Loop
		//int onDisplay;
		for(onDisplay=0;onDisplay<[displays count]&!error;onDisplay++)
		{
			[fullScreenContext makeCurrentContext];
			
			NSRect bounds=[[displays objectAtIndex:onDisplay] frame];
			NSRect virtualBounds=[displayData displays][onDisplay];
			
			// Render a frame, and swap the front and back buffers.
			// Tell the scene the dimensions of the area it's going to render to, so it can set up an appropriate viewport and viewing transformation.
			[scene setViewportRect:bounds virtualBounds:virtualBounds mainBounds:mainBounds];
			[views[onDisplay] updateTextString:bounds virtualBounds:virtualBounds mainBounds:mainBounds];
			
			[scene renderWithTextureTiler:[views[onDisplay] textureTiler]
							   helpString:[views[onDisplay] helpString]
							   textString:[views[onDisplay] textString]];
			[fullScreenContext flushBuffer];
		}
		
		// Clean up any autoreleased objects that were created this time through the loop.
		[pool release];
	}
	
	//Cleanup
	//int onDisplay;
	for(onDisplay=0;onDisplay<[displays count]&!error;onDisplay++)
	{
		if(fullScreenContext != nil)
		{
			[fullScreenContext makeCurrentContext];
			
			// Clear the front and back framebuffers before switching out of FullScreen mode.
			// (This is not strictly necessary, but avoids an untidy flash of garbage.)
			glClearColor(0.0,0.0,0.0,0.0);
			glClear(GL_COLOR_BUFFER_BIT);
			[fullScreenContext flushBuffer];
			glClear(GL_COLOR_BUFFER_BIT);
			[fullScreenContext flushBuffer];
			
			// Restore the previously set swap interval.
			CGLContextObj cglContext=CGLGetCurrentContext();
			CGLSetParameter(cglContext,kCGLCPSwapInterval,&oldSwapInterval[onDisplay]);
			
			// Exit fullscreen mode and release our FullScreen NSOpenGLContext.
			[NSOpenGLContext clearCurrentContext];
			
			[fullScreenContext clearDrawable];
			[fullScreenContext release];
			fullScreenContext = nil;
		}
	}
	// Release control of the display
	CGReleaseAllDisplays();
	[self setNeedsDisplay:YES];
	// Turn back on the animation timer
	[self updateAnimationTimers:[scene animating]];
#endif
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
	
	if ([self frame].size.width != size.width || [self frame].size.height != size.height)
	{
		[self reshape];
	}
	NSOpenGLContext *ctx = [self openGLContext];
	[ctx makeCurrentContext];
	m_pRenderWindow->Render();
	[ctx flushBuffer];
}

- (BOOL)go:(XMLNode*)rootNode {
	
	// Navigation Database (global)
	globals->m_NavDatabase->InitDatabase();
	
	// Create the data source
	XMLNode dsNode = rootNode->GetChild("DataSource");
	Assert(dsNode.HasProperty("type"), "DataSource node has no type property");
	string dsName = dsNode.GetProperty("type");
	const char *titleSuffix;
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
			return NO;
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
		return NO;
	}
	
	// Create the data calculations manager (CalcManager)
	m_CalcManager = new CalcManager();
	m_CalcManager->InitFromXMLNode(XMLNode() /*FIXME*/);
	
	// Set up the window title
	XMLNode windowNode = rootNode->GetChild("Window");
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
	m_pRenderWindow = new RenderWindow();
	globals->m_PrefManager->SetPrefD("UnitsPerPixel", m_pRenderWindow->GetUnitsPerPixel());
	
	// FIXME set windowX, windowY, windowTitle.c_str()
	
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
			return NO;
		}
		
		pGauge->InitFromXMLNode(*iter);
		m_pRenderWindow->AddGauge(pGauge);
	}
	
	m_pRenderWindow->ForceIsOKToRender(true);
	
	if (globals->m_PrefManager->GetPrefB("FullScreen"))
	{
		[self fullScreen];
	}
	
	// Set the update rate in nominal seconds per frame	
	animationInterval = globals->m_PrefManager->GetPrefD("AppUpdateRate");
	if (animationInterval > 0)
	{
		animationTimer = [NSTimer scheduledTimerWithTimeInterval:animationInterval
														  target:self
														selector:@selector(IdleFunction)
														userInfo:nil
														 repeats:YES];
	}
	
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
