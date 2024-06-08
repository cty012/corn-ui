#include <corn/media/image.h>
#include <corn/util/rich_text.h>
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

    const std::u8string& DOMNode::getText() const noexcept {
        return this->text_;
    }

    void DOMNode::setText(const std::u8string& text) noexcept {
        this->text_ = text;
        corn::UIWidget* widget = this->getWidget();
        if (!widget || widget->getType() != corn::UIType::LABEL) return;

        // Set the text
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
            ((corn::UILabel*)widget)->setText(corn::RichText().addText(
                    this->text_, corn::TextStyle(font, fontSize, fontColor, fontVariant)));
        }
    }

    const std::vector<std::string>& DOMNode::getClassList() const noexcept {
        return this->classList_;
    }

    const std::unordered_map<std::string, std::string>& DOMNode::getStyles() const noexcept {
        return this->styles_;
    }

    void DOMNode::setStyle(const std::string& name, const std::string& value) noexcept {
        this->styles_[name] = value;
        this->computeStyle();
    }

    const std::unordered_map<std::string, std::string>& DOMNode::getComputedStyles() const noexcept {
        return this->computedStyles_;
    }

    const std::unordered_map<std::string, std::string>& DOMNode::getAttributes() const noexcept {
        return this->attributes_;
    }

    void DOMNode::setAttributes(const std::string& name, const std::string& value) noexcept {
        if (name == "class" || name == "style") return;
        this->attributes_[name] = value;
        corn::UIWidget* widget = this->getWidget();
        if (!widget) return;

        if (widget->getType() == corn::UIType::IMAGE && name == "src") {
            auto* image = new corn::Image(this->dom_->getFile().parent_path() / value);
            ((corn::UIImage*)widget)->setImage(image);
        }
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
