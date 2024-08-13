#include <corn/media/image.h>
#include <corn/util/rich_text.h>
#include <corn/util/string_utils.h>
#include <cornui/util/reserved.h>
#include <cornui/xml/dom.h>
#include <cornui/xml/dom_node.h>

namespace cornui {
    const std::string& DOMNode::getTag() const noexcept {
        return this->tag_;
    }

    const std::string& DOMNode::getName() const noexcept {
        return this->name_;
    }

    void DOMNode::setName(const std::string& name) noexcept {
        this->name_ = name;
        corn::UIWidget* widget = this->getWidget();
        if (!widget) return;
        widget->setName(name);
    }

    const std::u8string& DOMNode::getLocalText() const noexcept {
        return this->text_;
    }

    std::u8string DOMNode::getText() const noexcept {
        if (!this->text_.empty()) {
            return this->text_;
        }

        std::u8string text;
        for (DOMNode* child: this->children_) {
            if (child->tag_ == "text") {
                text += child->getText();
            }
        }
        return text;
    }

    void DOMNode::setText(const std::u8string& text) noexcept {
        this->clearChildren();
        this->text_ = text;
        this->computeStyle();
    }

    corn::RichText DOMNode::getRichText() const {
        corn::RichText richText;

        // Only label and text nodes can have rich text
        if (this->tag_ != "label" && this->tag_ != "text") {
            return richText;
        }

        bool isLeaf = true;
        for (DOMNode* child: this->children_) {
            if (child->tag_ == "text") {
                isLeaf = false;
                break;
            }
        }

        if (isLeaf) {
            // Leaf nodes
            const corn::Font* font = corn::FontManager::instance().get(this->computedStyle_.at("font-family"));
            if (!font) {
                // todo: Use default font
                throw std::logic_error("Font not found: " + this->computedStyle_.at("font-family"));
            }
            size_t fontSize = std::stoi(this->computedStyle_.at("font-size"));
            corn::Color fontColor = corn::Color::parse(this->computedStyle_.at("font-color"));
            corn::FontVariant fontVariant = corn::FontVariant::REGULAR;
            if (this->computedStyle_.at("font-variant") == "bold") {
                fontVariant = corn::FontVariant::BOLD;
            } else if (this->computedStyle_.at("font-variant") == "italic") {
                fontVariant = corn::FontVariant::ITALIC;
            } else if (this->computedStyle_.at("font-variant") == "underline") {
                fontVariant = corn::FontVariant::UNDERLINE;
            }
            richText.addText(this->text_, corn::TextStyle(font, fontSize, fontColor, fontVariant));
        } else {
            // Non-leaf text nodes
            for (DOMNode* child: this->children_) {
                // Only text nodes are considered
                if (child->tag_ != "text") continue;

                corn::RichText childRichText = child->getRichText();
                for (const corn::RichText::Segment& segment: childRichText.segments) {
                    richText.addText(segment.str, segment.style);
                }
            }
        }

        return richText;
    }

    void DOMNode::setRichText(const corn::RichText& richText) {
        // Only label and text nodes can have rich text
        if (this->tag_ != "label" && this->tag_ != "text") {
            return;
        }

        // Clear all child text nodes
        std::vector<DOMNode*> toDelete;
        for (DOMNode* child: this->children_) {
            if (child->tag_ == "text") {
                delete child;
                toDelete.push_back(child);
            }
        }
        for (DOMNode* child: toDelete) {
            std::erase(this->children_, child);
        }

        // Store the rich text in its children
        for (const corn::RichText::Segment& segment: richText.segments) {
            auto* child = new DOMNode();
            child->tag_ = "text";
            child->text_ = segment.str;
            child->dom_ = this->dom_;
            child->parent_ = this;
            this->children_.push_back(child);
        }
    }

    const std::vector<std::string>& DOMNode::getClassList() const noexcept {
        return this->classList_;
    }

    const std::unordered_map<std::string, std::string>& DOMNode::getStyle() const noexcept {
        return this->style_;
    }

    void DOMNode::setStyle(const std::string& name, const std::string& value) noexcept {
        this->style_[name] = value;
        this->computeStyle();
    }

    const std::unordered_map<std::string, std::string>& DOMNode::getComputedStyle() const noexcept {
        return this->computedStyle_;
    }

    corn::Vec4 DOMNode::getComputedGeometry() const noexcept {
        if (!this->dom_ || !this->dom_->getUIManager() || this->widgetID_ == 0) {
            return {};
        }

        corn::UIManager& uiManager = *this->dom_->getUIManager();
        corn::UIWidget* widget = this->getWidget();
        return uiManager.getCachedGeometry(widget);
    }

    const std::unordered_map<std::string, std::string>& DOMNode::getAttributes() const noexcept {
        return this->attributes_;
    }

    void DOMNode::setAttribute(const std::string& name, const std::string& value) noexcept {
        if (attrIsReserved(name)) return;
        bool isScript = attrIsScript(name);

        this->attributes_[name] = isScript ? corn::trim(value) : value;
        corn::UIWidget* widget = this->getWidget();
        if (!widget) return;

        // src
        if (widget->getType() == corn::UIType::IMAGE && name == "src") {
            auto* image = new corn::Image(this->dom_->getFile().parent_path() / value);
            ((corn::UIImage*)widget)->setImage(image);
        }
    }

    void DOMNode::removeAttribute(const std::string& name) noexcept {
        if (name == "class" || name == "style") return;
        corn::UIWidget* widget = this->getWidget();
        if (!widget) return;

        // src
        if (widget->getType() == corn::UIType::IMAGE && name == "src") {
            return;
        }

        this->attributes_.erase(name);
    }

    std::unordered_map<std::string, DOMNode::Animation>& DOMNode::getAnimations() noexcept {
        return this->animations_;
    }

    DOM* DOMNode::getDOM() const noexcept {
        return this->dom_;
    }

    DOMNode* DOMNode::getParent() const noexcept {
        return this->parent_;
    }

    const std::vector<DOMNode*>& DOMNode::getChildren() const noexcept {
        return this->children_;
    }

    corn::UIWidget::WidgetID DOMNode::getWidgetID() const noexcept {
        return this->widgetID_;
    }

    corn::UIWidget* DOMNode::getWidget() const noexcept {
        if (this->widgetID_ == 0) return nullptr;
        corn::UIManager* uiManager = this->dom_->getUIManager();
        if (!uiManager) return nullptr;
        return uiManager->getWidgetByID(this->widgetID_);
    }
}
