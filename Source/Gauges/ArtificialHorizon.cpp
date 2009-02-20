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

#include <stdio.h>

#include "Globals.h"
#include "GaugeComponent.h"
#include "CircleEvaluator.h"
#include "ArtificialHorizon.h"

// Use the alternative "Gradient" colour scheme?
#define GRADIENT

namespace OpenGC
{

ArtificialHorizon::ArtificialHorizon()
{
	m_Font = globals->m_FontManager->LoadDefaultFont();

	m_PhysicalPosition.x = 0;
	m_PhysicalPosition.y = 0;

	m_PhysicalSize.x = 94;
	m_PhysicalSize.y = 98;

	m_Scale.x = 1.0;
	m_Scale.y = 1.0;
}

ArtificialHorizon::~ArtificialHorizon()
{

}

void ArtificialHorizon::Render()
{
	// Call base class to setup viewport and projection
	GaugeComponent::Render();

	// For drawing circles
	CircleEvaluator *aCircle = globals->m_CircleEvaluator;

	// First, store the "root" position of the gauge component
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	double Roll = globals->m_DataSource->GetAirframe()->GetRoll();
	double Pitch = globals->m_DataSource->GetAirframe()->GetPitch();

	// Move to the center of the window
	glTranslated(47,49,0);
	// Rotate based on the bank
	glRotated(Roll, 0, 0, 1);

	// Translate in the direction of the rotation based
	// on the pitch. On the 777, a pitch of 1 degree = 2 mm
	glTranslated(0, Pitch * -2.0, 0);

	//-------------------Gauge Background------------------
	// It's drawn oversize to allow for pitch and bank

	// The "ground" rectangle
	// Remember, the coordinate system is now centered in the gauge component

	glBegin(GL_POLYGON);

#ifdef GRADIENT
#define GROUND_COLOUR1()	glColor3ub(195,82,0)
#define GROUND_COLOUR2()	glColor3ub(253,88,0)
	GROUND_COLOUR1();
	glVertex2f(-75,-75);

	GROUND_COLOUR2();
	glVertex2f(-75,0);

	GROUND_COLOUR2();
	glVertex2f(75,0);

	GROUND_COLOUR1();
	glVertex2f(75,-75);

	GROUND_COLOUR1();
	glVertex2f(-75,-75);
#else
	glColor3ub(179,102,0);
	glVertex2f(-300,-300);
	glVertex2f(-300,0);
	glVertex2f(300,0);
	glVertex2f(300,-300);
	glVertex2f(-300,-300);
#endif
	glEnd();

	// The "sky" rectangle
	// Remember, the coordinate system is now centered in the gauge component

	glBegin(GL_POLYGON);

#ifdef GRADIENT
#define SKY_COLOUR1()	glColor3ub(68,195,255)
#define SKY_COLOUR2()	glColor3ub(30,71,247)
	SKY_COLOUR1();
	glVertex2f(-75,0);

	SKY_COLOUR2();
	glVertex2f(-75,75);

	SKY_COLOUR2();
	glVertex2f(75,75);

	SKY_COLOUR1();
	glVertex2f(75,0);

	SKY_COLOUR1();
	glVertex2f(-75,0);
#else
	glColor3ub(0,153,204);
	glVertex2f(-300,0);
	glVertex2f(-300,300);
	glVertex2f(300,300);
	glVertex2f(300,0);
	glVertex2f(-300,0);
#endif
	glEnd();

	//------------Draw the pitch markings--------------

	// Draw in white
	glColor3ub(255,255,255);
	// Specify line width
	glLineWidth(1.0);
	// The size for all pitch text
	globals->m_FontManager->SetSize(m_Font,4.0, 4.0);

	glBegin(GL_LINES);

	// The dividing line between sky and ground
//#ifdef GRADIENT
//	  glColor3ub(0,0,0);
//#endif
	glVertex2f(-100,0);
	glVertex2f(100,0);

//#ifdef GRADIENT
//	  glColor3ub(255,255,255);
//#endif

	// +2.5 degrees
	glVertex2f(-5,5);
	glVertex2f(5,5);

	// +5.0 degrees
	glVertex2f(-10,10);
	glVertex2f(10,10);

	// +7.5 degrees
	glVertex2f(-5,15);
	glVertex2f(5,15);

	// +10.0 degrees
	glVertex2f(-20,20);
	glVertex2f(20,20);

	// +12.5 degrees
	glVertex2f(-5,25);
	glVertex2f(5,25);

	// +15.0 degrees
	glVertex2f(-10,30);
	glVertex2f(10,30);

	// +17.5 degrees
	glVertex2f(-5,35);
	glVertex2f(5,35);

	// +20.0 degrees
	glVertex2f(-20,40);
	glVertex2f(20,40);

	// -2.5 degrees
	glVertex2f(-5,-5);
	glVertex2f(5,-5);

	// -5.0 degrees
	glVertex2f(-10,-10);
	glVertex2f(10,-10);

	// -7.5 degrees
	glVertex2f(-5,-15);
	glVertex2f(5,-15);

	// -10.0 degrees
	glVertex2f(-20,-20);
	glVertex2f(20,-20);

	// -12.5 degrees
	glVertex2f(-5,-25);
	glVertex2f(5,-25);

	// -15.0 degrees
	glVertex2f(-10,-30);
	glVertex2f(10,-30);

	// -17.5 degrees
	glVertex2f(-5,-35);
	glVertex2f(5,-35);

	// -20.0 degrees
	glVertex2f(-20,-40);
	glVertex2f(20,-40);

	glEnd();

	// +10
	globals->m_FontManager->Print(-27.5,18.0,"10",m_Font);
	globals->m_FontManager->Print(21.0,18.0,"10",m_Font);

	// -10
	globals->m_FontManager->Print(-27.5,-22.0,"10",m_Font);
	globals->m_FontManager->Print(21.0,-22.0,"10",m_Font);

	// +20
	globals->m_FontManager->Print(-27.5,38.0,"20",m_Font);
	globals->m_FontManager->Print(21.0,38.0,"20",m_Font);

	// -20
	globals->m_FontManager->Print(-27.5,-42.0,"20",m_Font);
	globals->m_FontManager->Print(21.0,-42.0,"20",m_Font);

#ifndef GRADIENT
	//-----The background behind the bank angle markings-------
	// Reset the modelview matrix
	glPopMatrix();
	glPushMatrix();

	// Draw in the sky color
	glColor3ub(0,153,204);

	aCircle->SetOrigin(47,49);
	aCircle->SetRadius(46);
	aCircle->SetDegreesPerPoint(5);
	aCircle->SetArcStartEnd(300.0,360.0);

	aCircle->ResetVertices();
	aCircle->AddVertex(0,98);
	aCircle->AddVertex(0,72);
	aCircle->Evaluate();
	aCircle->AddVertex(47,98);
	aCircle->Render(GL_TRIANGLE_FAN);

	aCircle->SetArcStartEnd(0.0,60.0);
	aCircle->ResetVertices();
	aCircle->AddVertex(94,98);
	aCircle->AddVertex(47,98);
	aCircle->Evaluate();
	aCircle->AddVertex(94,72);
	aCircle->Render(GL_TRIANGLE_FAN);
#endif

	//----------------The bank angle markings----------------

	// Left side bank markings
	// Reset the modelview matrix
	glPopMatrix();
	glPushMatrix();

	// Draw in white
	glColor3ub(255,255,255);

	// Move to the center of the window
	glTranslated(47,49,0);

	// Draw the center detent
	glBegin(GL_POLYGON);
	glVertex2f(0.0f,46.0f);
	glVertex2f(-2.3f,49.0f);
	glVertex2f(2.3f,49.0f);
	glVertex2f(0.0f,46.0f);
	glEnd();

	glRotated(10.0,0,0,1);
	glBegin(GL_LINES);
	glVertex2f(0,46);
	glVertex2f(0,49);
	glEnd();

	glRotated(10.0,0,0,1);
	glBegin(GL_LINES);
	glVertex2f(0,46);
	glVertex2f(0,49);
	glEnd();

	glRotated(10.0,0,0,1);
	glBegin(GL_LINES);
	glVertex2f(0,46);
	glVertex2f(0,53);
	glEnd();

	glRotated(15.0,0,0,1);
	glBegin(GL_LINES);
	glVertex2f(0,46);
	glVertex2f(0,49);
	glEnd();

	glRotated(15.0,0,0,1);
	glBegin(GL_LINES);
	glVertex2f(0,46);
	glVertex2f(0,51);
	glEnd();

	// Right side bank markings
	// Reset the modelview matrix
	glPopMatrix();
	glPushMatrix();
	// Move to the center of the window
	glTranslated(47,49,0);

	glRotated(-10.0,0,0,1);
	glBegin(GL_LINES);
	glVertex2f(0,46);
	glVertex2f(0,49);
	glEnd();

	glRotated(-10.0,0,0,1);
	glBegin(GL_LINES);
	glVertex2f(0,46);
	glVertex2f(0,49);
	glEnd();

	glRotated(-10.0,0,0,1);
	glBegin(GL_LINES);
	glVertex2f(0,46);
	glVertex2f(0,53);
	glEnd();

	glRotated(-15.0,0,0,1);
	glBegin(GL_LINES);
	glVertex2f(0,46);
	glVertex2f(0,49);
	glEnd();

	glRotated(-15.0,0,0,1);
	glBegin(GL_LINES);
	glVertex2f(0,46);
	glVertex2f(0,51);
	glEnd();

	//----------------End Draw Bank Markings----------------


	//----------------Bank Indicator----------------
	// Reset the modelview matrix
	glPopMatrix();
	glPushMatrix();
	// Move to the center of the window
	glTranslated(47,49,0);
	// Rotate based on the bank
	glRotated(Roll, 0, 0, 1);

	// Draw in white
	glColor3ub(255,255,255);
	// Specify line width
	glLineWidth(2.0);

	glBegin(GL_LINE_LOOP); // the bottom rectangle
	glVertex2f(-4.5, 39.5);
	glVertex2f(4.5, 39.5);
	glVertex2f(4.5, 41.5);
	glVertex2f(-4.5, 41.5);
	glEnd();

	glBegin(GL_LINE_STRIP); // the top triangle
	glVertex2f(-4.5, 41.5);
	glVertex2f(0, 46);
	glVertex2f(4.5, 41.5);
	glEnd();

	//----------------Flight Director----------------
	if (globals->m_DataSource->GetAirframe()->GetDirector_Active() == 1)
	{
		// Reset the modelview matrix
		glPopMatrix();
		glPushMatrix();

		// Get the data
		double directorPitch = globals->m_DataSource->GetAirframe()->GetDirector_Pitch();
		double directorRoll = globals->m_DataSource->GetAirframe()->GetDirector_Roll();

		// Move to the center of the window, move up/down for pitch, and rotate for roll
		glTranslated(47,49 + directorPitch * 2.0, 0);
		glRotated(directorRoll, 0.0, 0.0, 1.0);

		// Draw the wings symbol
		glColor3ub(255,0,255);
		glLineWidth(3.0);
		glBegin(GL_LINE_STRIP);
		glVertex2f(-20.0, 0.0);
		glVertex2f(-8.0,  0.0);
		glVertex2f(-4.0, -4.0);
		glVertex2f(0.0,  0.0);
		glVertex2f(4.0,  -4.0);
		glVertex2f(8.0,  0.0);
		glVertex2f(20.0, 0.0);
		glEnd();
	}

	//----------------Attitude Indicator----------------
	// Reset the modelview matrix
	glPopMatrix();
	glPushMatrix();
	// Move to the center of the window
	glTranslated(47,49,0);

	// The center axis indicator
	// Black background
	glColor3ub(0,0,0);
	glBegin(GL_POLYGON);
	glVertex2f(1.25,1.25);
	glVertex2f(1.25,-1.25);
	glVertex2f(-1.25,-1.25);
	glVertex2f(-1.25,1.25);
	glVertex2f(1.25,1.25);
	glEnd();
	// White lines
	glColor3ub(255,255,255);
	glLineWidth(2.0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(1.25,1.25);
	glVertex2f(1.25,-1.25);
	glVertex2f(-1.25,-1.25);
	glVertex2f(-1.25,1.25);
	glEnd();

	// The left part
	// Black background
	glColor3ub(0,0,0);
	glBegin(GL_POLYGON);
	glVertex2f(-39,1.25);
	glVertex2f(-19,1.25);
	glVertex2f(-19,-1.25);
	glVertex2f(-39,-1.25);
	glVertex2f(-39,1.25);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex2f(-19,1.25);
	glVertex2f(-19,-5.75);
	glVertex2f(-22,-5.75);
	glVertex2f(-22,1.25);
	glVertex2f(-19,1.25);
	glEnd();

	// White lines
	glColor3ub(255,255,255);
	glLineWidth(2.0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(-39,1.25);
	glVertex2f(-19,1.25);
	glVertex2f(-19,-5.75);
	glVertex2f(-22,-5.75);
	glVertex2f(-22,-1.25);
	glVertex2f(-39,-1.25);
	glEnd();

	// The right part
	// Black background
	glColor3ub(0,0,0);
	glBegin(GL_POLYGON);
	glVertex2f(39,1.25);
	glVertex2f(19,1.25);
	glVertex2f(19,-1.25);
	glVertex2f(39,-1.25);
	glVertex2f(39,1.25);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex2f(19,1.25);
	glVertex2f(19,-5.75);
	glVertex2f(22,-5.75);
	glVertex2f(22,1.25);
	glVertex2f(19,1.25);
	glEnd();

	// White lines
	glColor3ub(255,255,255);
	glLineWidth(2.0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(39,1.25);
	glVertex2f(19,1.25);
	glVertex2f(19,-5.75);
	glVertex2f(22,-5.75);
	glVertex2f(22,-1.25);
	glVertex2f(39,-1.25);
	glEnd();
	//--------------End draw attitude indicator------------
	// Reset the modelview matrix
	glPopMatrix();
	glPushMatrix();

	aCircle->SetRadius(3.77);
	//-------------------Rounded corners------------------
	// The corners of the artificial horizon are rounded off by
	// drawing over them in black. The overlays are essentially the
	// remainder of a circle subtracted from a square, and are formed
	// by fanning out triangles from a point just off each corner
	// to an arc descrbing the curved portion of the art. horiz.
	
	// Note we draw each rounded corner as a line too, for antialiasing.

	glColor3ub(0,0,0);
	glLineWidth(1.0);
	// Lower left
	aCircle->SetOrigin(3.77,3.77);
	aCircle->SetArcStartEnd(180,270);
	aCircle->SetDegreesPerPoint(15);
	aCircle->ResetVertices();
	aCircle->AddVertex(-1.0,-1.0);
	aCircle->Evaluate();
	aCircle->Render(GL_TRIANGLE_FAN);
	aCircle->Render(GL_LINE_STRIP);
	// Upper left
	aCircle->SetOrigin(3.77,94.23);
	aCircle->SetArcStartEnd(270,360);
	aCircle->SetDegreesPerPoint(15);
	aCircle->ResetVertices();
	aCircle->AddVertex(-1.0,99.0);
	aCircle->Evaluate();
	aCircle->Render(GL_TRIANGLE_FAN);
	aCircle->Render(GL_LINE_STRIP);
	// Upper right
	aCircle->SetOrigin(90.23,94.23);
	aCircle->SetArcStartEnd(0,90);
	aCircle->SetDegreesPerPoint(15);
	aCircle->ResetVertices();
	aCircle->AddVertex(95.0,99.0);
	aCircle->Evaluate();
	aCircle->Render(GL_TRIANGLE_FAN);
	aCircle->Render(GL_LINE_STRIP);
	//Lower right
	aCircle->SetOrigin(90.23,3.77);
	aCircle->SetArcStartEnd(90,180);
	aCircle->SetDegreesPerPoint(15);
	aCircle->ResetVertices();
	aCircle->AddVertex(95.0,-1);
	aCircle->Evaluate();
	aCircle->Render(GL_TRIANGLE_FAN);
	aCircle->Render(GL_LINE_STRIP);

	// Finally, restore the modelview matrix to what we received
	glPopMatrix();
}

} // end namespace OpenGC
