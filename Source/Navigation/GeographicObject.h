/*=========================================================================

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

/**
 * The base class for all objects that occupy a location on the surface of the earth
 */

#ifndef GeographicObject_h
#define GeographicObject_h

#include <stdio.h>
#include <string>

namespace OpenGC
{

using namespace std;

class GeographicObject
{
	public:
		GeographicObject();
		virtual ~GeographicObject();

		/** Set physical parameters */
		void SetDegreeLat(double degree) {m_DegreeLat = degree;}
		void SetDegreeLon(double degree) {m_DegreeLon = degree;}
		void SetAltitudeMeters(double alt) { m_AltitudeMeters = alt; }

		/** Set mercator map coordinates */
		void SetMercatorMeters(double n, double e) {m_MercatorNorthingMeters = n; m_MercatorEastingMeters = e;}

		/** Get mercator map coordinates */
		void GetMercatorMeters(double &n, double &e) {n = m_MercatorNorthingMeters; e = m_MercatorEastingMeters;}

		/** Set names */
		void SetIdentification(const string& s) { m_Identification = s; }
		void SetFullname(const string& s) { m_FullName = s; }

		/** Accessors for physical parameters */
		double GetAltitudeMeters() { return m_AltitudeMeters; }
		double GetDegreeLat() { return m_DegreeLat; }
		double GetDegreeLon() { return m_DegreeLon; }

		/** Accessors for IDs */
		string GetIdentification() { return m_Identification; }
		string GetFullName() { return m_FullName; };

		/** Convert Lat/Lon into Mercator Meters */
		static void LatLonToMercator(double lat, double lon, 
				double &northing, double &easting); 

	protected:
		/** Physical parameters */
		double m_DegreeLat, m_DegreeLon, m_AltitudeMeters;

		/** Mercator map coordinates in meters */
		double m_MercatorNorthingMeters, m_MercatorEastingMeters;

		/** Brief name, e.g. "KPIT" */
		string m_Identification;

		/** Full name, e.g. "Pittsburgh International Airport" */
		string m_FullName;
};

} // end namespace OpenGC

#endif

