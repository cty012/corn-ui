#include <stdexcept>
#include <utility>
extern "C" {
#include <libxml/parser.h>
}
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
}
