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

#ifndef NavDisplay_h
#define NavDisplay_h

#include "Gauge.h"

namespace OpenGC
{

class NavDisplay : public Gauge
{
	public:

		NavDisplay();
		virtual ~NavDisplay();

		void Render();

		/** Called if the down mouse click applies to this object */
		void OnMouseDown(int button, double physicalX, double physicalY);

	protected:

		//////////// Graphics Functions ///////////////////////////////////////

		void PlotCourse();
		void PlotWaypoints();
		void PlotWindSpeedDirection();
		void PlotMap();

		/** Display geographic objects such as airports or navaids on the map */
		void PlotGeoObjs(std::list<GeographicObject*>& geoList);

		//////////// Helper functions /////////////////////////////////////////
		
		/** Convert mercator coordinates in meters into pixels relative to
		 *  gauge center (where the aircraft is) */
		void PointToPixelCoord(double objNorthing, double objEasting, double &xPos, double &yPos);

		/** Check if an object is visible */
		bool PixelCoordIsVisible(double xPos, double yPos);
		
		//////////// Variables ////////////////////////////////////////////////
		
		/** The height (and width) of the component in nautical miles */
		double m_SizeNM;

		/** The font number provided to us by the font manager */
		int m_Font;

		/** Coordinates etc. used in graphics functions */
		double aircraftLat, aircraftLon, aircraftHeading;
		double mercatorNorthing, mercatorEasting;
		
		/** Compile-time layout options */
		static const double CENTER_X = 90.0, CENTER_Y = 95.0, OVERLAY_Y = 45.0;
		static const int compass_interval = 20; // interval in degrees between compass markings
	
		/** Raster map tile texture handles */
		static GLuint m_TileTextures[49];
		static bool m_TilesInitted;
	
};

} // end namespace OpenGC

#endif
