#include <corn/media/image.h>
#include <corn/util/rich_text.h>
#include <corn/util/string_utils.h>
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
        const corn::Font* font = corn::FontManager::instance().get(this->computedStyle_["font-family"]);
        size_t fontSize = std::stoi(this->computedStyle_["font-size"]);
        corn::Color fontColor = corn::Color::parse(this->computedStyle_["font-color"]);
        corn::FontVariant fontVariant = corn::FontVariant::REGULAR;
        if (this->computedStyle_["font-variant"] == "bold") {
            fontVariant = corn::FontVariant::BOLD;
        } else if (this->computedStyle_["font-variant"] == "italic") {
            fontVariant = corn::FontVariant::ITALIC;
        } else if (this->computedStyle_["font-variant"] == "underline") {
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

    const std::unordered_map<std::string, std::string>& DOMNode::getAttributes() const noexcept {
        return this->attributes_;
    }

    void DOMNode::setAttribute(const std::string& name, const std::string& value) noexcept {
        static const std::vector<std::string> scripts = { "onclick", "onhover", "onenter", "onexit", "onscroll" };
        if (name == "class" || name == "style") return;
        bool isScript = std::find(scripts.begin(), scripts.end(), name) != scripts.end();

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
