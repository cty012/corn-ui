#include <format>
#include <sstream>
extern "C" {
#include <libxml/parser.h>
#include <libxml/xmlreader.h>
}
#include <corn/util/string_utils.h>
#include <cornui/util/css_parser.h>
#include <cornui/util/reserved.h>
#include <cornui/xml/dom.h>
#include <cornui/xml/dom_node.h>
#include "xml_helper.h"

namespace cornui {
    std::string DOMNode::getInnerXML() const noexcept {
        std::stringstream ss;
        if (this->children_.empty()) {
            // If is a text node
            ss << reinterpret_cast<const char*>(this->text_.c_str());
        } else {
            // If is an element node
            for (const DOMNode* child: this->children_) {
                ss << child->getOuterXML();
            }
        }
        return ss.str();
    }

    std::string DOMNode::getOuterXML() const noexcept {
        std::stringstream ss;

        // Opening tag & name
        ss << "<" << this->tag_ << " name=\"" << this->name_ << "\"";

        // Class list
        if (!this->classList_.empty()) {
            ss << " class=\"";
            for (size_t i = 0; i < this->classList_.size(); i++) {
                if (i) ss << " ";
                ss << this->classList_[i];
            }
            ss << "\"";
        }
        // Other attributes
        for (auto& [name, val] : this->attributes_) {
            ss << " " << name << "=\"" << val << "\"";
        }
        // Closing tag
        ss << ">" << this->getInnerXML() << "</" << this->tag_ << ">";
        return ss.str();
    }

    // Parse the content as an array of XML nodes
    void DOMNode::setInnerXML(const std::string& content) {
        // If the node is a text node, set the inner XML as text
        if (this->tag_ == "label" || this->tag_ == "text") {
            this->setInnerXMLAsText(content);
            return;
        }

        // Clear the children
        this->clearChildren();

        // Create a new xmlTextReader for the input string
        std::string wrappedContent = std::format("<{}>{}</{}>", this->tag_, content, this->tag_);
        xmlTextReaderPtr reader = xmlReaderForMemory(wrappedContent.c_str(), (int)wrappedContent.size(), nullptr, nullptr, 0);
        if (reader == nullptr) {
            // Error parsing the XML content
            throw std::invalid_argument(std::format(
                    "Failed to parse the inner XML content from:\n{}\n",
                    content));
        }

        xmlTextReaderRead(reader);  // Read the dummy root node
        int ret = xmlTextReaderRead(reader);  // Read the first child node
        while (ret == 1) {
            // Check if the current node is an element node
            if (xmlTextReaderNodeType(reader) == XML_READER_TYPE_ELEMENT) {
                // Create a new child node to store the element
                xmlChar* outerXML = xmlTextReaderReadOuterXml(reader);
                this->children_.push_back(new DOMNode(DOMNode::fromOuterXML(this->dom_, (const char*)outerXML, false, false)));
                xmlFree(outerXML);
            }

            // Move to the next element
            ret = xmlTextReaderNext(reader);
        }

        // Update to reflect the changes
        this->computeStyle();
    }

    void DOMNode::setInnerXMLAsText(const std::string& content) noexcept {
        // If the node is not a text node, do nothing
        if (this->tag_ != "label" && this->tag_ != "text") {
            return;
        }

        // Clear
        this->text_.clear();
        this->clearChildren();

        // Create a new xmlTextReader for the input string
        std::string wrappedContent = std::format("<{}>{}</{}>", this->tag_, content, this->tag_);
        xmlTextReaderPtr reader = xmlReaderForMemory(wrappedContent.c_str(), (int)wrappedContent.size(), nullptr, nullptr, 0);
        if (reader == nullptr) {
            // Error parsing the XML content
            return;
        }

        xmlTextReaderRead(reader);  // Read the dummy root node

        int ret = xmlTextReaderRead(reader);  // Read the first child node
        size_t nodeCount = 0;
        while (ret == 1) {
            nodeCount++;

            // Check if the current node is an element node
            if (xmlTextReaderNodeType(reader) == XML_READER_TYPE_ELEMENT) {
                // Skip nodes that are not text nodes
                const char* tag = (const char*)xmlTextReaderConstName(reader);
                if (!tag || strcmp(tag, "text") != 0) {
                    ret = xmlTextReaderNext(reader);
                    continue;
                }

                // Create a new child node to store the element
                xmlChar* outerXML = xmlTextReaderReadOuterXml(reader);
                this->children_.push_back(new DOMNode(DOMNode::fromOuterXML(this->dom_, (const char*)outerXML, false, true)));
                xmlFree(outerXML);
            } else if (xmlTextReaderNodeType(reader) == XML_READER_TYPE_TEXT) {
                const auto* text = (const char8_t*)xmlTextReaderConstValue(reader);

                ret = xmlTextReaderNext(reader);
                if (nodeCount == 1 && (ret != 1 || xmlTextReaderNodeType(reader) == XML_READER_TYPE_END_ELEMENT)) {
                    // If this is the only text node, directly store the text in the current node
                    this->text_ = text;
                } else {
                    // Create a new child node to store the text
                    auto* child = new DOMNode();
                    child->clear();
                    child->dom_ = this->dom_;
                    this->children_.push_back(child);

                    // Add the text directly to child
                    child->tag_ = "text";
                    child->text_ = text;
                }

                continue;
            } else if (xmlTextReaderNodeType(reader) == XML_READER_TYPE_END_ELEMENT) {
                break;
            }

            // Move to the end of the current element
            ret = xmlTextReaderNext(reader);
        }

        // Update to reflect the changes
        this->computeStyle();
    }

    DOMNode DOMNode::fromOuterXML(DOM* dom, const std::string& outerXML, bool isDefNode, bool allowTextNode) {
        DOMNode domNode;

        // Create a new xmlTextReader for the input string
        xmlTextReaderPtr reader = xmlReaderForMemory(outerXML.c_str(), (int)outerXML.size(), nullptr, nullptr, 0);
        if (reader == nullptr) {
            // Error parsing the XML content
            return {};
        }
        int ret = xmlTextReaderRead(reader);
        if (ret != 1) {
            throw std::invalid_argument(std::format(
                    "Failed to read the outer XML content from:\n{}\n",
                    outerXML));
        }

        domNode.clear();
        domNode.dom_ = dom;

        // Set the tag
        const char* tag = (const char*)xmlTextReaderConstName(reader);
        if (tag) {
            domNode.tag_ = reinterpret_cast<const char*>(tag);
        }
        if (!allowTextNode && domNode.tag_ == "text") {
            throw std::invalid_argument("Text nodes are not allowed outside label nodes.");
        }
        bool isBuiltinNode = tagIsReserved(domNode.tag_);
        bool shouldUseDefTemplate = !isBuiltinNode && !isDefNode;

        // If tag is not a built-in tag and is not currently defining a def node, check for def tags
        if (shouldUseDefTemplate) {
            const std::unordered_map<std::string, DOM::Def>& defs = dom->getDefs();
            if (defs.contains(domNode.tag_)) {
                // Copy the def node's content
                domNode = defs.at(domNode.tag_).node;
            } else {
                throw std::invalid_argument(std::format(
                        "Tag definition not found for <{}>.",
                        domNode.tag_));
            }
        }

        // Set the attributes
        getAttributesFromReader(reader, domNode.attributes_);

        // Set the name
        if (domNode.attributes_.contains("name")) {
            domNode.name_ = domNode.attributes_["name"];
            domNode.attributes_.erase("name");
        }

        // Set the class list
        if (domNode.attributes_.contains("class")) {
            std::istringstream iss(domNode.attributes_["class"]);
            std::string token;
            while (iss >> token) {
                // Ignore duplicates
                if (std::find(domNode.classList_.begin(), domNode.classList_.end(), token) == domNode.classList_.end()) {
                    domNode.classList_.push_back(token);
                }
            }
            domNode.attributes_.erase("class");
        }

        // Set the style
        if (domNode.attributes_.contains("style")) {
            std::unordered_map<std::string, std::string> styleTemp = parseDeclFromString(domNode.attributes_["style"]);
            for (auto& [name, value] : styleTemp) {
                domNode.style_[name] = value;
            }
            domNode.attributes_.erase("style");
        }

        // Script attributes
        for (auto& [name, value] : domNode.attributes_) {
            if (attrIsScript(name)) {
                domNode.attributes_[name] = corn::trim(value);
            }
        }

        // Set the innerXML of the node
        if (!shouldUseDefTemplate) {
            xmlChar* innerXML = xmlTextReaderReadInnerXml(reader);
            domNode.setInnerXML((char*)innerXML);
            xmlFree(innerXML);
        }

        return domNode;
    }
}
