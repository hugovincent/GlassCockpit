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

/**
 * AppObject is the functional piece of the OpenGC example program.
 * It is responsible for creating gauges and allocating/deallocating
 * the various objects used in OpenGC.
 */

#ifndef AppObject_h
#define AppObject_h

#include "CalcManager.h"
#include "FLTKRenderWindow.h"
#include "Messageable.h"
#include "XMLNode.h"

namespace OpenGC
{

class AppObject : public Messageable
{
	public:
		AppObject();
		virtual ~AppObject();

		/** Setup and run the glass cockpit - enters message loop */
		bool Go(XMLNode rootNode);

		/** The idle function used to repeatedly update the display */
		void IdleFunction();

		/** Handler to receive messages */
		virtual void OnMessage(Message message, void *data);

	private:

		/** Main pieces of the application */
		FLTKRenderWindow* m_pRenderWindow;

		/** Object to manage AirframeData variables calculated from other variables */
		CalcManager* m_CalcManager;
};

} // end namespace OpenGC

#endif

