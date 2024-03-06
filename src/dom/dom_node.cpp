#include <sstream>
extern "C" {
#include <libxml/parser.h>
}
#include <corn/util/string_utils.h>
#include <cornui/dom/dom_node.h>
#include "dom_helper.h"

namespace cornui {
    void clearNode(DOMNode* node) {
        if (!node) return;
        for (DOMNode* child : node->children) {
            clearNode(child);
            delete child;
        }
        node->tag = u8"";
        node->text = u8"";
        node->classList.clear();
        node->attributes.clear();
        node->children.clear();
    }

    DOMNode* dupNode(const DOMNode* node) {
        auto* dup = new DOMNode();
        dup->tag = node->tag;
        dup->text = node->text;
        dup->classList = node->classList;
        dup->attributes = node->attributes;
        dup->children.clear();
        for (const DOMNode* child : node->children) {
            dup->children.push_back(dupNode(child));
        }
        return dup;
    }

    std::u8string getInnerXML(const DOMNode& node) {
        std::stringstream ss;
        ss << reinterpret_cast<const char*>(node.text.c_str());
        for (const DOMNode* child : node.children) {
            ss << reinterpret_cast<const char*>(getOuterXML(*child).c_str());
        }
        return reinterpret_cast<const char8_t*>(ss.str().c_str());
    }

    std::u8string getOuterXML(const DOMNode& node) {
        std::stringstream ss;
        // Opening tag
        ss << "<" << reinterpret_cast<const char*>(node.tag.c_str());
        // Class list
        if (!node.classList.empty()) {
            ss << " class=\"";
            for (size_t i = 0; i < node.classList.size(); i++) {
                if (i) ss << " ";
                ss << reinterpret_cast<const char*>(node.classList[i].c_str());
            }
            ss << "\"";
        }
        // Attributes
        for (auto& [name, val] : node.attributes) {
            ss << " " << reinterpret_cast<const char*>(name.c_str()) << "=\""
               << reinterpret_cast<const char*>(val.c_str()) << "\"";
        }
        // Closing tag
        ss << ">" << reinterpret_cast<const char*>(getInnerXML(node).c_str()) << "</"
           << reinterpret_cast<const char*>(node.tag.c_str()) << ">";
        return reinterpret_cast<const char8_t*>(ss.str().c_str());
    }

    void loadXMLToNode(xmlNodePtr xmlNode, DOMNode& node) {
        if (xmlNode->type != XML_ELEMENT_NODE) {
            throw std::invalid_argument("xmlNode must have type XML_ELEMENT_NODE");
        }
        clearNode(&node);

        // Copy tag
        node.tag = reinterpret_cast<const char8_t*>(xmlNode->name);

        // Copy text
        std::stringstream ssText;
        for (xmlNodePtr xmlChild = xmlNode->children; xmlChild; xmlChild = xmlChild->next) {
            if (xmlChild->type == XML_TEXT_NODE) {
                ssText << (char*)xmlChild->content;
            }
        }
        std::string temp = ssText.str();
        corn::trim(temp);
        node.text = reinterpret_cast<const char8_t*>(corn::trim(ssText.str()).c_str());

        // Copy class list and attributes
        for (xmlAttr* attr = xmlNode->properties; attr; attr = attr->next) {
            const auto* name = reinterpret_cast<const char*>(attr->name);
            const auto* u8name = reinterpret_cast<const char8_t*>(attr->name);
            xmlChar* xmlValue = xmlNodeGetContent(attr->children);
            const auto* value = reinterpret_cast<const char*>(xmlValue);
            const auto* u8value = reinterpret_cast<const char8_t*>(xmlValue);
            if (strcmp(name, "class") == 0) {
                // Split the value and fill the class list
                std::istringstream iss(value);
                std::string token;
                while (iss >> token) {
                    node.classList.emplace_back(reinterpret_cast<const char8_t*>(token.c_str()));
                }
            } else {
                // Insert the attribute
                node.attributes[u8name] = u8value;
            }
            xmlFree(xmlValue);
        }

        // Copy children
        for (xmlNodePtr xmlChild = xmlNode->children; xmlChild; xmlChild = xmlChild->next) {
            if (xmlChild->type == XML_ELEMENT_NODE) {
                auto* child = new DOMNode();
                loadXMLToNode(xmlChild, *child);
                node.children.push_back(child);
            }
        }
    }
}
