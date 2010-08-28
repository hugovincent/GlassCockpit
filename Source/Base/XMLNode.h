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

#ifndef XMLNode_h
#define XMLNode_h

#include <list>
#include <string>
#include <libxml/tree.h>

#ifndef LIBXML_TREE_ENABLED
#error "XMLNode requires LIBXML_TREE_ENABLED (fatal)."
#endif

using namespace std;

/** XMLNode encapsulates the xmlNode type from libxml-2, and wraps it in 
 * a more object-oriented API. */
class XMLNode
{
	public:
		XMLNode();
		~XMLNode();
		
		/** Initialize with an existing libxml-2 xmlNode */
		XMLNode(xmlNode *node);

		//////////// Child Nodes //////////////////////////////////////////////
		/** A list of XMLNodes */
		typedef list<XMLNode> NodeList;

		/** Return a list of all children nodes */
		NodeList GetChildList();
		
		/** Return a list of named children nodes */
		NodeList GetChildList(const string& name);

		/** Determine if this node has a child node */
		bool HasChild(const string& name);

		/** Get a child node. If this node has no such child, the returned
		 * XMLNode will be have IsValid() == false. */
		XMLNode GetChild(const string& name);

		//////////// Node Contents ////////////////////////////////////////////
		/** Is this node valid, correctly initialized, and refers to a node
		 * that exists */
		bool IsValid();
		
		/** Get the node name */
		string GetName();

		/** Get the text contents of this node */
		string GetText();

		/** Get the text contents as a double */
		double GetTextAsDouble();

		/** Get the text contents as an int */
		int GetTextAsInt();

		/** Get the text contents as a boolean */
		bool GetTextAsBool();

		/** Get the text as a pair of doubles. The text needs to be in the 
		 * format x,y where x, y are two doubles */
		void GetTextAsCoord(double &x, double &y);

		//////////// Properties/Attributes ////////////////////////////////////
		/** Check if this node has a property */
		bool HasProperty(const string& name);

		/** Get the property text */
		string GetProperty(const string& name);

	private:
		/** The libxml-2 XML node structure for this element */
		xmlNode *m_Node;
};

#endif

