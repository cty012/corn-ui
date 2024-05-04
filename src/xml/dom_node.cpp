#include <cstring>
#include <sstream>
extern "C" {
#include <libxml/parser.h>
}
#include <corn/media/image.h>
#include <corn/util/rich_text.h>
#include <corn/util/string_utils.h>
#include <cornui/css/cssom.h>
#include <cornui/xml/dom.h>
#include <cornui/xml/dom_node.h>

namespace cornui {
    DOMNode::DOMNode() noexcept : dom_(nullptr), parent_(nullptr), widgetID_(0) {}

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

    DOMNode::DOMNode(const DOMNode& other) noexcept : DOMNode() {
        this->tag_ = other.tag_;
        this->name_ = other.name_;
        this->text_ = other.text_;
        this->classList_ = other.classList_;
        this->styles_ = other.styles_;
        this->attributes_ = other.attributes_;
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
        this->dom_ = nullptr;
        this->parent_ = nullptr;
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

    bool DOMNode::addClass(const std::string& className) noexcept {
        // Return false if already exist
        if (std::find(this->classList_.begin(), this->classList_.end(), className) != this->classList_.end()) {
            return false;
        }
        this->classList_.push_back(className);
        this->computeStyle();
        return true;
    }

    bool DOMNode::removeClass(const std::string& className) noexcept {
        bool success = std::erase(this->classList_, className);
        if (success) this->computeStyle();
        return success;
    }

    void DOMNode::computeStyle() {
        // Reset computedStyles to default
        this->computedStyles_ = {
                { "active", "true" },
                { "x", "0px" }, { "y", "0px" }, { "w", "100%nw" }, { "h", "100%nh" },
                { "z-order", "0" },
                { "overflow", "display" },
                { "background", "#ffffff00" },
                { "opacity", "255" },
                { "font-family", "noto-sans" },
                { "font-size", "16" },
                { "font-color", "#000000" },
                { "font-variant", "regular" }
        };

        // Inherit
        for (const auto& [name, value] : this->inheritedStyles_) {
            this->computedStyles_[name] = value;
        }

        // Styles from stylesheet
        for (const CSSRule& style : this->getDOM()->getCSSOM().getRules()) {
            // Apply the styles if the selector matches the current node
            if (match(style.selector, *this)) {
                for (const auto& [name, value] : style.declarations) {
                    this->computedStyles_[name] = value;
                }
            }
        }

        // Inline styles
        for (const auto& [name, value] : this->styles_) {
            this->computedStyles_[name] = value;
        }

        // Apply styles to the UIWidgets
        if (this->widgetID_ > 0) {
            corn::UIWidget* widget = this->dom_->getUIManager()->getWidgetByID(this->widgetID_);
            if (widget != nullptr) {
                // Apply general styles
                widget->setName(this->name_);
                widget->setActive(this->computedStyles_["active"] == "true");
                widget->setX(this->computedStyles_["x"]);
                widget->setY(this->computedStyles_["y"]);
                widget->setW(this->computedStyles_["w"]);
                widget->setH(this->computedStyles_["h"]);
                widget->setZOrder(std::stoi(this->computedStyles_["z-order"]));
                if (this->computedStyles_["overflow"] == "display") {
                    widget->setOverflow(corn::UIOverflow::DISPLAY);
                } else if (this->computedStyles_["overflow"] == "hidden") {
                    widget->setOverflow(corn::UIOverflow::HIDDEN);
                }
                widget->setBackground(corn::Color::parse(this->computedStyles_["background"]));
                widget->setOpacity((unsigned char)std::stoi(this->computedStyles_["opacity"]));

                // Apply widget type-specific styles
                if (this->tag_ == "label") {
                    const corn::Font* font = corn::FontManager::instance().get(this->computedStyles_["font-family"]);
                    size_t fontSize = std::stoi(this->computedStyles_["font-size"]);
                    corn::Color fontColor = corn::Color::parse(this->computedStyles_["font-color"]);
                    corn::FontVariant fontVariant = corn::FontVariant::REGULAR;
                    if (this->computedStyles_["font-variant"] == "bold") {
                        fontVariant = corn::FontVariant::BOLD;
                    } else if (this->computedStyles_["font-variant"] == "italic") {
                        fontVariant = corn::FontVariant::ITALIC;
                    } else if (this->computedStyles_["font-variant"] == "underline") {
                        fontVariant = corn::FontVariant::UNDERLINE;
                    }

                    if (font) {
                        ((corn::UILabel*) widget)->setText(corn::RichText().addText(
                                this->text_, corn::TextStyle(font, fontSize, fontColor, fontVariant)));
                    }
                } else if (this->tag_ == "image") {
                    auto* image = new corn::Image(this->dom_->getFile().parent_path() / this->attributes_["src"]);
                    ((corn::UIImage*)widget)->setImage(image);
                }
            }
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
