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
        // todo: sync changes
    }

    const std::u8string& DOMNode::getText() const noexcept {
        return this->text_;
    }

    void DOMNode::setText(const std::u8string& text) noexcept {
        this->text_ = text;
        // todo
    }

    const std::unordered_map<std::string, std::string>& DOMNode::getStyles() const noexcept {
        return this->styles_;
    }

    void DOMNode::setStyle(const std::string& name, const std::string& value) noexcept {
        this->styles_[name] = value;
        this->computeStyle();
        // todo
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
        // todo
    }

    const std::vector<DOMNode*>& DOMNode::getChildren() const noexcept {
        return this->children_;
    }

}
