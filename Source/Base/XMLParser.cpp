#include <iostream>
#include <ctype.h>
#include "XMLParser.h"
#include "Debug.h"

using namespace std;

XMLParser::XMLParser() 
{
	// This initializes the library and checks that the ABI version matches
	// the API version etc.
	LIBXML_TEST_VERSION

	xmlInitParser();
}

XMLParser::~XMLParser()
{
	if (m_XMLDoc != NULL) {
		xmlFreeDoc(m_XMLDoc); // note this recursively free's all the nodes too
		m_XMLDoc = NULL;
		m_XMLRootNode = NULL;
	}

	xmlCleanupParser();
}

bool XMLParser::ReadFile(const string& filename)
{
	// Parse the file and get the DOM
	m_XMLDoc = xmlReadFile(filename.c_str(), NULL, 0);

	if (m_XMLDoc == NULL) {
		cerr << "Error: could not parse XML file \"" << filename << "\".\n";
		return false;
	}

	// Get the root element node
	m_XMLRootNode = xmlDocGetRootElement(m_XMLDoc);

	if (m_XMLRootNode == NULL) {
		cerr << "Error: empty XML file \"" << filename << "\".\n";
		return false;
	}

	if (!CheckFileType()) {
		cerr << "Error: incorrect XML file type.\n";
		return false;
	}

	return true;
}

bool XMLParser::IsWhiteSpace(const string& str)
{
	string::const_iterator iter;
	for (iter = str.begin(); iter != str.end(); ++iter)
		if (!isspace((*iter)))
			return false;

	return true;
}

void XMLParser::PrintTree(xmlNode *aNode, string indentation)
{
	// FIXME maybe use the XMLNode API instead of direct xmlNode manipulation

	xmlNode *curNode = aNode;

	// Deal with entry into the top of the recursion
	if (aNode == NULL)
		curNode = m_XMLRootNode;
	
	// Now iterate and recurse
	for (; curNode; curNode = curNode->next) {
		if (curNode->type == XML_ELEMENT_NODE) {
			// Get the node name
			cout << indentation << curNode->name;

			// Get the properties/attributes
			for (xmlAttr *prop = curNode->properties; prop; prop = prop->next) {
				const xmlChar *val = xmlNodeGetContent(prop->children); 
				cout << " (" << prop->name << "=\"" << val << "\")";
				xmlFree((void*)val);
			}

			// Get the data/content
			const xmlChar *content = xmlNodeGetContent(curNode->children); 
			if (content != NULL && !IsWhiteSpace(string((const char*)content)))
				cout << " = \"" << content << "\"\n";
			else
				cout << "\n";
			xmlFree((void*)content);
			
			if (curNode->children != NULL) {
				PrintTree(curNode->children, indentation + "    ");
			}
		}
	}
}

XMLNode XMLParser::GetNode(const string& path)
{
	XMLNode node = XMLNode(m_XMLRootNode);
	unsigned long start_offset = 1, end_offset = 1;

	// First, check the path starts with "/"
	// (We don't handle relative paths here)
	Assert(path[0] == '/', "XMLNode path must start with '/'");
	if (path == "/")
		return node; // At this point, node is m_XMLRootNode

	// Now iterate over each token in the path and get its node
	while (start_offset < path.size())
	{
		// Find next "/" if there is one, otherwise the end of the string
		end_offset = path.find('/', start_offset);
		end_offset = (end_offset == string::npos) ? path.size() : end_offset;
	
		// Extract the "token" between the two slashes
		string tok = path.substr(start_offset, end_offset - start_offset);

		// Get the corresponding node
		node = node.GetChild(tok);
		if (!node.IsValid())
		{
			break; // In this case, the invalid node is returned
		}
		
		start_offset = end_offset + 1;
	}

	return node;
}

