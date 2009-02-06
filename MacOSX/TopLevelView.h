//
//  TopLevelView.h
//  GlassCockpit
//
//  Created by Hugo Vincent on 4/02/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#include "CalcManager.h"
#include "XMLNode.h"
#include "RenderWindow.h"

@interface TopLevelView : NSOpenGLView {
@private
	OpenGC::RenderWindow* m_pRenderWindow;
	OpenGC::CalcManager* m_CalcManager;
	
	NSSize size;
	NSTimer *animationTimer;
    NSTimeInterval animationInterval;
}

@end
