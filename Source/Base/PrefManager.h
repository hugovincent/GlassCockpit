/*=========================================================================

  OpenGC - The Open Source Glass Cockpit Project
  Please see our web site at http://www.opengc.org

  Albatross UAV Project - http://www.albatross-uav.org

  Copyright (c) 2006 Hugo Vincent.
  All rights reserved.
  See Copyright.txt or http://www.opengc.org/Copyright.htm for details.

  This software is distributed WITHOUT ANY WARRANTY; without even 
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
  PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#ifndef PrefManager_h
#define PrefManager_h

#include <map>
#include "XMLNode.h"

namespace OpenGC {

/** Global manager for preferences in the application. Preferences are stored
 * in an STL map, where the XML node name for each preference (i.e. a string)
 * is the key. See preferences.xml to see or change the default preferences,
 * or default.xml (or your setup XML file) to change your specific preference
 * values. */
class PrefManager
{
	public:
		PrefManager();
		~PrefManager();

		/** Initialise the Preferences map from an XML file */
		void InitPreferences(const char* xmlFileName);

		/** Set several preferences from an XML file <Preferences> section */
		void SetPrefsFromXML(XMLNode prefNode);
			
		/** Get pref value as a string, boolean, double or integer */
		string GetPrefS(const string& prefKey);
		bool GetPrefB(const string& prefKey);
		double GetPrefD(const string& prefKey);
		int GetPrefI(const string& prefKey);

		/** Set pref value as string, integer, boolean or double */
		void SetPrefS(const string& prefKey, const string value);
		void SetPrefI(const string& prefKey, const int value);
		void SetPrefB(const string& prefKey, const bool value);
		void SetPrefD(const string& prefKey, const double value);

		/** Print all preferences for debugging */
		void PrintAll() const;

	private:
		/** The structure that configuration options are stored in. */
		typedef struct {
			string	asString;
			double	asDouble;
			bool	asBool;
			int		asInt;
			bool	isSet; // set to true when initialised
			char	type;  // 'I'=int, 'B'=bool, 'D'=double, 'S'=string
		} Preference;

		std::map<string, Preference*> m_PrefMap;
};

} // end namespace OpenGC

#endif
