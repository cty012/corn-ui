#include <queue>
#include <stdexcept>
extern "C" {
#include <libxml/parser.h>
}
#include <corn/event/event_args.h>
#include <corn/media.h>
#include <corn/ui.h>
#include <corn/util/rich_text.h>
#include <corn/util/string_utils.h>
#include <cornui/css/cssom.h>
#include <cornui/util/css_parser.h>
#include <cornui/xml/dom.h>

namespace cornui {
    DOM::DOM(UI* ui) : ui_(ui), uiManager_(nullptr) {}

    void DOM::bind(corn::UIManager& uiManager) {
        // First, store the UI manager to be referenced later
        this->uiManager_ = &uiManager;
        uiManager.clear();

        std::function<void(corn::UIManager&, const corn::UIWidget*, DOMNode&)> loadWidgetFromDOMNode =
                [&](corn::UIManager& uiManager, const corn::UIWidget* parent, DOMNode& domNode) {
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
                                domNode.name_, parent,
                                new corn::Image(50, 50, corn::Color::rgb(255, 255, 255, 0)));
                    }

                    // Invalid DOM node
                    if (!current) return;
                    domNode.widgetID_ = current->getID();

                    // onclick, onhover, onmouseenter, onmouseexit
                    if (domNode.attributes_.contains("onclick") && !domNode.attributes_.at("onclick").empty()) {
                        current->setClickable(true);
                    }
                    current->getEventManager().addListener(
                            "corn::ui::onclick", [&domNode, current](const corn::EventArgs& args) {
                                const auto& args_ = dynamic_cast<const corn::EventArgsUIOnClick&>(args);
                                if (args_.target == current && args_.mousebtnEvent.status == corn::ButtonEvent::UP) {
                                    domNode.runScriptInAttr("onclick");
                                }
                            });
                    current->getEventManager().addListener(
                            "corn::ui::onhover", [&domNode](const corn::EventArgs&) {
                                domNode.runScriptInAttr("onhover");
                            });
                    current->getEventManager().addListener(
                            "corn::ui::onenter", [&domNode](const corn::EventArgs&) {
                                domNode.runScriptInAttr("onmouseenter");
                            });
                    current->getEventManager().addListener(
                            "corn::ui::onexit", [&domNode](const corn::EventArgs&) {
                                domNode.runScriptInAttr("onmouseexit");
                            });

                    // Load to children
                    for (DOMNode* child : domNode.children_) {
                        loadWidgetFromDOMNode(uiManager, current, *child);
                    }
                };

        // Load all children
        for (DOMNode* child : this->root_.children_) {
            loadWidgetFromDOMNode(uiManager, nullptr, *child);
        }

        // Finally compute and apply the styles
        this->root_.computeStyle();
    }

    DOMNode* DOM::getNodeBySelector(const CSSSelector& selector) const {
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

    std::vector<DOMNode*> DOM::getNodesBySelector(const CSSSelector& selector) const {
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

    DOMNode* DOM::getNodeBySelector(const std::string& selector) const {
        return this->getNodeBySelector(parseSelectorFromString(selector));
    }

    std::vector<DOMNode*> DOM::getNodesBySelector(const std::string& selector) const {
        return this->getNodesBySelector(parseSelectorFromString(selector));
    }

    const UI& DOM::getUI() const noexcept {
        return *this->ui_;
    }

    UI& DOM::getUI() noexcept {
        return *this->ui_;
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

    void DOM::init(const std::filesystem::path& file, std::vector<std::filesystem::path>& toLoad) {
        this->file_ = file;

        xmlInitParser();
        xmlDocPtr doc = xmlReadFile(file.string().c_str(), nullptr, 0);
        if (doc == nullptr) {
            throw std::invalid_argument("Cannot load file '" + file.string() + "'!\n");
        }

        // Root node must be <cornui>
        xmlNodePtr root = xmlDocGetRootElement(doc);
        const char* rootTag = reinterpret_cast<const char*>(root->name);
        if (strcmp(rootTag, "cornui") != 0) {
            throw std::invalid_argument("File '" + file.string() + "' has root element with tag '<"
                                        + rootTag + ">', '<cornui>' expected!\n");
        }

        // Find <head> and <body>
        xmlNodePtr head = nullptr, body = nullptr;
        for (xmlNodePtr xmlChild = root->children; xmlChild; xmlChild = xmlChild->next) {
            if (xmlChild->type == XML_ELEMENT_NODE) {
                const char* tag = reinterpret_cast<const char*>(xmlChild->name);
                if (!strcmp(tag, "head")) {
                    // Avoid multiple heads
                    if (head) throw std::invalid_argument("File '" + file.string() + "' has multiple <head>s.");
                    head = xmlChild;
                } else if (!strcmp(tag, "body")) {
                    // Avoid multiple heads
                    if (body) throw std::invalid_argument("File '" + file.string() + "' has multiple <body>s.");
                    body = xmlChild;
                }
            }
        }

        // If either not found
        if (!head && !body) {
            throw std::invalid_argument("File '" + file.string() + "' has no <head> and <body>.");
        } else if (!head) {
            throw std::invalid_argument("File '" + file.string() + "' has no <head>.");
        } else if (!body) {
            throw std::invalid_argument("File '" + file.string() + "' has no <body>.");
        }

        // Load head
        // todo: possibly load <def> when feature is added
        for (xmlNodePtr xmlChild = head->children; xmlChild; xmlChild = xmlChild->next) {
            if (xmlChild->type == XML_ELEMENT_NODE) {
                const char* tag = reinterpret_cast<const char*>(xmlChild->name);
                if (!strcmp(tag, "style")) {
                    for (xmlAttr* attr = xmlChild->properties; attr; attr = attr->next) {
                        const char* name = reinterpret_cast<const char*>(attr->name);
                        xmlChar* xmlValue = xmlNodeGetContent(attr->children);
                        const char* value = reinterpret_cast<const char*>(xmlValue);
                        if (strcmp(name, "src") == 0) {
                            this->cssom_.loadFromFile(file.parent_path() / value);
                        }
                        xmlFree(xmlValue);
                    }
                } else if (!strcmp(tag, "script")) {
                    for (xmlAttr* attr = xmlChild->properties; attr; attr = attr->next) {
                        const char* name = reinterpret_cast<const char*>(attr->name);
                        xmlChar* xmlValue = xmlNodeGetContent(attr->children);
                        const char* value = reinterpret_cast<const char*>(xmlValue);
                        if (strcmp(name, "src") == 0) {
                            toLoad.push_back(file.parent_path() / value);
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
}
