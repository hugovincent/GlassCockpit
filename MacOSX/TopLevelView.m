//
//  TopLevelView.m
//  GlassCockpit
//
//  Created by Hugo Vincent on 4/02/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "TopLevelView.h"
#include <OpenGL/gl.h>

@implementation TopLevelView

static void drawAnObject ()
{
    glColor3f(1.0f, 0.85f, 0.35f);
    glBegin(GL_TRIANGLES);
    {
        glVertex3f(  0.0,  0.6, 0.0);
        glVertex3f( -0.2, -0.3, 0.0);
        glVertex3f(  0.2, -0.3 ,0.0);
    }
    glEnd();
}

- (void)drawRect:(NSRect)rect {

    glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	drawAnObject();
	glFlush();
}

@end
