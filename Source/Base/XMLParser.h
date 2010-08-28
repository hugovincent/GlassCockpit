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

#ifndef XMLParser_h
#define XMLParser_h

#include <string>
#include <libxml/parser.h>
#include "XMLNode.h"

using namespace std;

/** XMLParser is a thin wrapper around libxml-2, supporting reading, parsing
 * and extracting data from XML files. It is intended that classes to read
 * specific types of XML documents (such as PropertyLists) inherit from this
 * class, adding extra validation and data extraction methods.
 */
class XMLParser
{
	public:
		XMLParser();
		~XMLParser();

		/** Read an XML file into the parser */
		bool ReadFile(const string& filename);

		/** Heirarchically print the tree for debugging.
		 * Call as PrintTree(0, 0) - its recursive. */
		void PrintTree(xmlNode *aNode, string indentation);

		/** Check if the XML document has a particular node */
		bool HasNode(const string& path) { return GetNode(path).IsValid(); }

		/** Get a particular node from the XML document
		 * Note: should be a unique path, otherwise the first
		 * matching node will be found */
		XMLNode GetNode(const string& path);

	protected:
		/** The libxml-2 XML document structure */
		xmlDoc *m_XMLDoc;

		/** The root (top level) node of the XML document */
		xmlNode *m_XMLRootNode;

		/** Overload to change what types of documents are accepted,
		 * for example, by checking what the top level node is named */
		bool CheckFileType() { return true; }

		/** Checks if a string is wholely white space. */
		static bool IsWhiteSpace(const string& str);
};

#endif

