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

