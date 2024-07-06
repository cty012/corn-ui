#include "xml_helper.h"

namespace cornui {
    std::string getInnerXMLFromNode(xmlNodePtr node) {
        xmlBufferPtr buffer = xmlBufferCreate();
        if (buffer == nullptr) {
            // Failed to create the buffer
            return "";
        }

        // Serialize the child nodes of the root element
        for (xmlNodePtr child = node->children; child != nullptr; child = child->next) {
            xmlNodeDump(buffer, node->doc, child, 0, 0);
        }

        // Cleanup
        std::string innerXML = (const char*)buffer->content;
        xmlBufferFree(buffer);

        return innerXML;
    }

    std::string getOuterXMLFromNode(xmlNodePtr node) {
        xmlBufferPtr buffer = xmlBufferCreate();
        if (buffer == nullptr) {
            // Failed to create the buffer
            return "";
        }

        xmlNodeDump(buffer, node->doc, node, 0, 0);

        // Cleanup
        std::string outerXML = (const char*)buffer->content;
        xmlBufferFree(buffer);

        return outerXML;
    }

    void getAttributesFromReader(xmlTextReaderPtr reader, std::unordered_map<std::string, std::string>& attributes) {
        if (xmlTextReaderHasAttributes(reader)) {
            xmlTextReaderMoveToFirstAttribute(reader);
            do {
                const xmlChar* name = xmlTextReaderConstName(reader);
                const xmlChar* value = xmlTextReaderConstValue(reader);
                if (name && value) {
                    attributes[(const char*)name] = (const char*)value;
                }
            } while (xmlTextReaderMoveToNextAttribute(reader));
            xmlTextReaderMoveToElement(reader); // Move back to the element node
        }
    }
}
