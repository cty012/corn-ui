#include <stdexcept>
#include <utility>
extern "C" {
#include <libxml/parser.h>
}
#include <corn/ui.h>
#include <corn/util.h>
#include <cornui/dom/dom_tree.h>
#include "dom_helper.h"

namespace cornui {
    DOMTree::DOMTree(std::string file) : file_(std::move(file)) {
        xmlInitParser();
        xmlDocPtr doc = xmlReadFile(this->file_.c_str(), nullptr, 0);
        (void)doc;
        if (doc == nullptr) {
            throw std::invalid_argument("Cannot load file!\n");
        }
        loadXMLToNode(xmlDocGetRootElement(doc), this->root_);
        xmlFreeDoc(doc);
    }

    DOMTree::~DOMTree() {
        clearNode(&this->root_);
    }

    DOMTree::DOMTree(const DOMTree& other) {
        this->file_ = other.file_;
        auto* dup = dupNode(&other.root_);
        this->root_ = *dup;
        delete dup;
    }

    DOMTree& DOMTree::operator=(const DOMTree& other) {
        if (this == &other) return *this;
        clearNode(&this->root_);
        this->file_ = other.file_;
        auto* dup = dupNode(&other.root_);
        this->root_ = *dup;
        delete dup;
        return *this;
    }

    DOMTree::DOMTree(DOMTree&& other) noexcept {
        this->file_ = std::move(other.file_);
        this->root_ = other.root_;
        other.root_.children.clear();
        clearNode(&other.root_);
    }

    DOMTree& DOMTree::operator=(DOMTree&& other) noexcept {
        if (this == &other) return *this;
        clearNode(&this->root_);
        this->file_ = std::move(other.file_);
        this->root_ = other.root_;
        other.root_.children.clear();
        clearNode(&other.root_);
        return *this;
    }

    DOMNode& DOMTree::getRoot() noexcept {
        return this->root_;
    }

    const DOMNode& DOMTree::getRoot() const noexcept {
        return this->root_;
    }

    /// @brief Helper to the function below.
    void loadWidgetFromDOMNode(corn::UIManager& uiManager, const corn::UIWidget* parent, const DOMNode& domNode) {
        corn::UIWidget* current = nullptr;
        // Load to current node
        if (domNode.tag == "widget") {
            current = uiManager.createWidget<corn::UIWidget>(domNode.name, parent);
        } else if (domNode.tag == "label") {  // @todo: find a way to encode richtext in xml
            const corn::Font* font = corn::FontManager::instance().get("noto-sans-zh");
            uiManager.createWidget<corn::UILabel>(domNode.name, parent, corn::RichText().addText(
                    domNode.text, corn::TextStyle(font, 24)));
        }

        // Invalid DOM node
        if (!current) return;

        // Load to children
        for (const DOMNode* child : domNode.children) {
            loadWidgetFromDOMNode(uiManager, current, *child);
        }
    }

    void loadUIFromDOM(corn::UIManager& uiManager, const DOMTree& dom) {
        uiManager.clear();
        loadWidgetFromDOMNode(uiManager, nullptr, dom.getRoot());
    }
}
