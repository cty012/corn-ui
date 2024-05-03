#include <queue>
#include <stdexcept>
#include <utility>
extern "C" {
#include <libxml/parser.h>
}
#include <corn/media.h>
#include <corn/ui.h>
#include <corn/util/rich_text.h>
#include <corn/util/string_utils.h>
#include <cornui/css/cssom.h>
#include <cornui/util/css_parser.h>
#include <cornui/xml/dom.h>

namespace cornui {
    DOM::DOM(std::filesystem::path file) : file_(std::move(file)) {
        xmlInitParser();
        xmlDocPtr doc = xmlReadFile(this->file_.string().c_str(), nullptr, 0);
        if (doc == nullptr) {
            throw std::invalid_argument("Cannot load file '" + this->file_.string() + "'!\n");
        }

        // Root node must be <cornui>
        xmlNodePtr root = xmlDocGetRootElement(doc);
        const char* rootTag = reinterpret_cast<const char*>(root->name);
        if (strcmp(rootTag, "cornui") != 0) {
            throw std::invalid_argument("File '" + this->file_.string() + "' has root element with tag '<"
                                        + rootTag + ">', '<cornui>' expected!\n");
        }

        // Find <head> and <body>
        xmlNodePtr head = nullptr, body = nullptr;
        for (xmlNodePtr xmlChild = root->children; xmlChild; xmlChild = xmlChild->next) {
            if (xmlChild->type == XML_ELEMENT_NODE) {
                const char* tag = reinterpret_cast<const char*>(xmlChild->name);
                if (!strcmp(tag, "head")) {
                    // Avoid multiple heads
                    if (head) throw std::invalid_argument("File '" + this->file_.string() + "' has multiple <head>s.");
                    head = xmlChild;
                } else if (!strcmp(tag, "body")) {
                    // Avoid multiple heads
                    if (body) throw std::invalid_argument("File '" + this->file_.string() + "' has multiple <body>s.");
                    body = xmlChild;
                }
            }
        }

        // If either not found
        if (!head && !body) {
            throw std::invalid_argument("File '" + this->file_.string() + "' has no <head> and <body>.");
        } else if (!head) {
            throw std::invalid_argument("File '" + this->file_.string() + "' has no <head>.");
        } else if (!body) {
            throw std::invalid_argument("File '" + this->file_.string() + "' has no <body>.");
        }

        // Load head
        // todo: load <script> (and possibly <def>)
        for (xmlNodePtr xmlChild = head->children; xmlChild; xmlChild = xmlChild->next) {
            if (xmlChild->type == XML_ELEMENT_NODE) {
                const char* tag = reinterpret_cast<const char*>(xmlChild->name);
                if (!strcmp(tag, "style")) {
                    for (xmlAttr* attr = xmlChild->properties; attr; attr = attr->next) {
                        const char* name = reinterpret_cast<const char*>(attr->name);
                        xmlChar* xmlValue = xmlNodeGetContent(attr->children);
                        const char* value = reinterpret_cast<const char*>(xmlValue);
                        if (strcmp(name, "src") == 0) {
                            this->cssom_.loadFromFile(this->file_.parent_path() / value);
                        }
                        xmlFree(xmlValue);
                    }
                }
            }
        }

        // Load body
        // helper function
#include "dom_helper.h"

        loadXMLBodyToNode(body, this->root_);
        xmlFreeDoc(doc);
    }

    void DOM::bind(corn::UIManager& uiManager) {
        // First, store the UI manager to be referenced later
        this->uiManager_ = &uiManager;
        uiManager.clear();

        std::function<void(corn::UIManager&, const corn::UIWidget*, DOMNode&, const std::filesystem::path&)> loadWidgetFromDOMNode =
                [&](corn::UIManager& uiManager, const corn::UIWidget* parent, DOMNode& domNode, const std::filesystem::path& file) {
                    corn::UIWidget* current = nullptr;
                    // Load to current node
                    if (domNode.tag_ == "widget") {
                        current = &uiManager.createWidget<corn::UIWidget>(domNode.name_, parent);
                    } else if (domNode.tag_ == "label") {  // @todo: find a way to encode richtext in xml
                        const corn::Font* font = corn::FontManager::instance().get("noto-sans-zh");
                        current = &uiManager.createWidget<corn::UILabel>(domNode.name_, parent, corn::RichText().addText(
                                domNode.text_, corn::TextStyle(font, 16)));
                    } else if (domNode.tag_ == "image") {
                        current = &uiManager.createWidget<corn::UIImage>(
                                domNode.name_, parent, new corn::Image(file));
                    }

                    // Invalid DOM node
                    if (!current) return;
                    domNode.widgetID_ = current->getID();

                    // Load to children
                    for (DOMNode* child : domNode.children_) {
                        loadWidgetFromDOMNode(uiManager, current, *child, file);
                    }
                };

        // Load all children
        for (DOMNode* child : this->root_.children_) {
            loadWidgetFromDOMNode(uiManager, nullptr, *child, this->file_);
        }

        // Finally compute and apply the styles
        this->root_.computeStyle();
    }

    DOMNode* DOM::getNodeBySelecter(const CSSSelector& selector) const {
        std::queue<DOMNode*> toCheck;
        toCheck.push(const_cast<DOMNode*>(&this->root_));

        while (!toCheck.empty()) {
            DOMNode* current = toCheck.front();
            toCheck.pop();
            if (!current) continue;

            // If current node is a match, return the current node
            if (match(selector, *current)) {
                return current;
            }

            // Otherwise check the child nodes
            for (DOMNode* child : current->children_) {
                toCheck.push(child);
            }
        }

        return nullptr;
    }

    std::vector<DOMNode*> DOM::getNodesBySelecter(const CSSSelector& selector) const {
        std::queue<DOMNode*> toCheck;
        std::vector<DOMNode*> result;
        toCheck.push(const_cast<DOMNode*>(&this->root_));

        while (!toCheck.empty()) {
            DOMNode* current = toCheck.front();
            toCheck.pop();
            if (!current) continue;

            // If current node is a match, add the current node to the list of results
            if (match(selector, *current)) {
                result.push_back(current);
            }

            // Check the child nodes
            for (DOMNode* child : current->children_) {
                toCheck.push(child);
            }
        }

        return result;
    }

    const std::filesystem::path& DOM::getFile() const noexcept {
        return this->file_;
    }

    DOMNode& DOM::getRoot() noexcept {
        return this->root_;
    }

    const DOMNode& DOM::getRoot() const noexcept {
        return this->root_;
    }

    CSSOM& DOM::getCSSOM() noexcept {
        return this->cssom_;
    }

    const CSSOM& DOM::getCSSOM() const noexcept {
        return this->cssom_;
    }

    corn::UIManager* DOM::getUIManager() noexcept {
        return this->uiManager_;
    }

    const corn::UIManager* DOM::getUIManager() const noexcept {
        return this->uiManager_;
    }
}
