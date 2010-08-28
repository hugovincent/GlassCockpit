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

#include "PrefManager.h"
#include "XMLParser.h"
#include "Debug.h"

namespace OpenGC {

PrefManager::PrefManager()
{

}

PrefManager::~PrefManager()
{
	std::map<string, Preference*>::iterator iter;
	for (iter = m_PrefMap.begin(); iter != m_PrefMap.end(); ++iter)
	{
		delete (*iter).second;
	}
}

void PrefManager::SetPrefsFromXML(XMLNode prefNode)
{
	// Load in XML values for the preferences
	Check(prefNode.IsValid() && prefNode.GetName() == "Preferences");
	XMLNode::NodeList nodeList = prefNode.GetChildList();
	XMLNode::NodeList::iterator iter;
	for (iter = nodeList.begin(); iter != nodeList.end(); ++iter)
	{
		string prefKey = (*iter).GetName();
		LogPrintf("PrefManager: setting %s = %s\n", prefKey.c_str(), 
				(*iter).GetText().c_str());

		// Set the Preference
		Preference *toSet = m_PrefMap[prefKey];
		if (toSet->type == 'D')
		{
			SetPrefD(prefKey, (*iter).GetTextAsDouble());
		}
		else if (toSet->type == 'S')
		{
			SetPrefS(prefKey, (*iter).GetText());
		}
		else if (toSet->type == 'I')
		{
			SetPrefI(prefKey, (*iter).GetTextAsInt());
		}
		else if (toSet->type == 'B')
		{
			SetPrefB(prefKey, (*iter).GetTextAsBool());
		}
	}
}

void PrefManager::PrintAll() const
{
	printf("PrefManager: database contains %d entries:\n", (int)m_PrefMap.size());

	std::map<string, Preference*>::const_iterator iter;
	for (iter = m_PrefMap.begin(); iter != m_PrefMap.end(); ++iter)
	{
		Preference *p = (*iter).second;

		if (p->type == 'D')
		{
			printf("\t%s = [%s] %f\n", (*iter).first.c_str(),
					"double", p->asDouble);
		}
		else if (p->type == 'S')
		{
			printf("\t%s = [%s] \"%s\"\n", (*iter).first.c_str(),
					"string", p->asString.c_str());
		}
		else if (p->type == 'B')
		{
			printf("\t%s = [%s] %i\n", (*iter).first.c_str(),
					"bool", p->asBool);
		}
		else if (p->type == 'I')
		{
			printf("\t%s = [%s] %i\n", (*iter).first.c_str(),
					"integer", p->asInt);
		}
	}
}

void PrefManager::InitPreferences(const char* xmlFileName)
{
	XMLParser parser;
	Assert(parser.ReadFile(xmlFileName), "unable to read XML file");
	Check(parser.HasNode("/"));
	XMLNode rootNode = parser.GetNode("/");
	Check(rootNode.IsValid() && rootNode.GetName() == "Preferences");

	// Iterate over the preference definitions
	XMLNode::NodeList nodeList = rootNode.GetChildList();
	XMLNode::NodeList::iterator iter;
	for (iter = nodeList.begin(); iter != nodeList.end(); ++iter)
	{
		XMLNode node = *iter;
		Check(node.GetName() == "Preference");
		Check(node.HasChild("Name") && node.HasChild("Type") &&
				node.HasChild("DefaultValue"));
		
		// Now create each Preference struct
		Preference* p = new Preference;
		string type = node.GetChild("Type").GetText();
		if (type == "double")
		{
			p->type = 'D';
			p->asDouble = node.GetChild("DefaultValue").GetTextAsDouble();
		}
		else if (type == "string")
		{
			p->type = 'S';
			p->asString = node.GetChild("DefaultValue").GetText();
		}
		else if (type == "integer")
		{
			p->type = 'I';
			p->asInt = node.GetChild("DefaultValue").GetTextAsInt();
		}
		else if (type == "boolean")
		{
			p->type = 'B';
			p->asBool = node.GetChild("DefaultValue").GetTextAsBool();
		}
		p->isSet = true;
		m_PrefMap[node.GetChild("Name").GetText()] = p;
	}
}

bool PrefManager::GetPrefB(const string& prefKey)
{
	const Preference* pref = m_PrefMap[prefKey];
	Assert(pref != NULL, "no such preference");
	Assert(pref->isSet && pref->type == 'B', "getting un-set Preference");
	return pref->asBool;
}

int PrefManager::GetPrefI(const string& prefKey) 
{
	const Preference* pref = m_PrefMap[prefKey];
	Assert(pref != NULL, "no such preference");
	Assert(pref->isSet && pref->type == 'I', "getting un-set Preference");
	return pref->asInt;
}

double PrefManager::GetPrefD(const string& prefKey)
{
	const Preference* pref = m_PrefMap[prefKey];
	Assert(pref != NULL, "no such preference");
	Assert(pref->isSet && pref->type == 'D', "getting un-set Preference");
	return pref->asDouble;
}

string PrefManager::GetPrefS(const string& prefKey)
{
	const Preference* pref = m_PrefMap[prefKey];
	Assert(pref != NULL, "no such preference");
	Assert(pref->isSet && pref->type == 'S', "getting un-set Preference");
	return pref->asString;
}

void PrefManager::SetPrefB(const string& prefKey, const bool value)
{
	Preference* pref = m_PrefMap[prefKey];
	if (pref->isSet)
	{
		Assert(pref->type == 'B', "setting preference with wrong type");
		pref->asBool = value;
	}
	else
	{
		pref->isSet = true;
		pref->type = 'B';
		pref->asBool = value;
	}
}

void PrefManager::SetPrefD(const string& prefKey, const double value)
{
	Preference* pref = m_PrefMap[prefKey];
	if (pref->isSet)
	{
		Assert(pref->type == 'D', "setting preference with wrong type");
		pref->asDouble = value;
	}
	else
	{
		pref->isSet = true;
		pref->type = 'D';
		pref->asDouble = value;
	}
}

void PrefManager::SetPrefS(const string& prefKey, const string value)
{
	Preference* pref = m_PrefMap[prefKey];
	if (pref->isSet)
	{
		Assert(pref->type == 'S', "setting preference with wrong type");
		pref->asString = value;
	}
	else
	{
		pref->isSet = true;
		pref->type = 'S';
		pref->asString = value;
	}
}

void PrefManager::SetPrefI(const string& prefKey, const int value)
{
	Preference* pref = m_PrefMap[prefKey];
	if (pref->isSet)
	{
		Assert(pref->type == 'S', "setting preference with wrong type");
		pref->asInt = value;
	}
	else
	{
		pref->isSet = true;
		pref->type = 'S';
		pref->asInt = value;
	}
}

} // end namespace OpenGC
