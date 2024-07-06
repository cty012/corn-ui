#pragma once

#include <string>
extern "C" {
#include <libxml/parser.h>
#include <libxml/xmlreader.h>
}
#include <cornui/xml/dom_node.h>

namespace cornui {
    // Function to get the inner XML of a node using xmlNode
    std::string getInnerXMLFromNode(xmlNodePtr node);

    // Function to get the outer XML of a node using xmlNode
    std::string getOuterXMLFromNode(xmlNodePtr node);

    // Function to get the attributes of a node using xmlTextReader
    void getAttributesFromReader(xmlTextReaderPtr reader, std::unordered_map<std::string, std::string>& attributes);
}
