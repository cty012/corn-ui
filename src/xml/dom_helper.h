#pragma once

extern "C" {
#include <libxml/parser.h>
}
#include <cornui/xml/dom_node.h>

namespace cornui {
    /**
     * Helper function for loading from a xmlNodePtr to a dom node. Automatically releases node's descendants.
     * @param xmlNode The XML data to load. Must have type XML_ELEMENT_NODE.
     * @param node The target node to load into.
     */
    void loadXMLToNode(xmlNodePtr xmlNode, DOMNode& node);
}
