#include <cstring>
#include <sstream>
extern "C" {
#include <libxml/parser.h>
}
#include <corn/util/string_utils.h>
#include <cornui/xml/dom_node.h>

namespace cornui {
    DOMNode::DOMNode() noexcept : widgetID_(0) {}

    DOMNode::~DOMNode() {
        this->clear();
    }

    void DOMNode::clear() noexcept {
        for (DOMNode* child : this->children_) {
            child->clear();
            delete child;
        }
        this->tag_ = "";
        this->name_ = "";
        this->text_ = u8"";
        this->classList_.clear();
        this->styles_.clear();
        this->inheritedStyles_.clear();
        this->computedStyles_.clear();
        this->attributes_.clear();
        this->children_.clear();
        this->widgetID_ = 0;
    }

    DOMNode::DOMNode(const DOMNode& other) noexcept {
        this->tag_ = other.tag_;
        this->name_ = other.name_;
        this->text_ = other.text_;
        this->classList_ = other.classList_;
        this->styles_ = other.styles_;
        this->attributes_ = other.attributes_;
        this->widgetID_ = 0;
        for (const DOMNode* child : this->children_) {
            this->children_.push_back(new DOMNode(*child));
        }
    }

    DOMNode& DOMNode::operator=(const DOMNode& other) noexcept {
        if (&other == this) return *this;
        this->clear();
        this->tag_ = other.tag_;
        this->name_ = other.name_;
        this->text_ = other.text_;
        this->classList_ = other.classList_;
        this->styles_ = other.styles_;
        this->attributes_ = other.attributes_;
        this->widgetID_ = 0;
        for (const DOMNode* child : this->children_) {
            this->children_.push_back(new DOMNode(*child));
        }
        return *this;
    }

    std::string DOMNode::getInnerXML() const noexcept {
        std::stringstream ss;
        ss << reinterpret_cast<const char*>(this->text_.c_str());
        for (const DOMNode* child : this->children_) {
            ss << child->getOuterXML();
        }
        return ss.str();
    }

    std::string DOMNode::getOuterXML() const noexcept {
        std::stringstream ss;
        // Opening tag
        ss << "<" << this->tag_;
        // Name
        ss << " name=\"" << this->name_ << "\"";
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

    void DOMNode::computeStyle() {
        // Inherit
        this->computedStyles_ = this->inheritedStyles_;

        // Styles from stylesheet
        // todo

        // Inline styles
        for (const auto& [name, value] : this->styles_) {
            this->computedStyles_[name] = value;
        }

        // Compute styles of child nodes
        for (DOMNode* child : this->children_) {
            child->computeStyle(this->computedStyles_);
        }
    }

    void DOMNode::computeStyle(const std::unordered_map<std::string, std::string>& inheritedStyles) {
        for (const std::string& styleName : { "font-family", "font-size", "font-color", "font-variant" }) {
            if (inheritedStyles.contains(styleName)) {
                this->inheritedStyles_[styleName] = inheritedStyles.at(styleName);
            }
        }
        this->computeStyle();
    }
}
