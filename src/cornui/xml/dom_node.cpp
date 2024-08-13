#include <format>
#include <duktape.h>
#include <corn/core/scene.h>
#include <corn/media/image.h>
#include <corn/util/rich_text.h>
#include <cornui/css/animation.h>
#include <cornui/css/cssom.h>
#include <cornui/js/runtime.h>
#include <cornui/ui.h>
#include <cornui/util/key.h>
#include <cornui/xml/dom.h>
#include <cornui/xml/dom_node.h>
#include "../css/event_animation.h"
#include "../js/runtime_impl.h"
#include "../js/dom_node.h"
#include "../util/style_name.h"

namespace cornui {
    std::string DOMNode::Animation::getCurrentValue() const {
        // Edge case: invalid time
        if (this->totalTime <= 0.0f || this->currentTime < 0.0f || this->currentTime >= this->totalTime) {
            return this->endValue;
        }

        // Find the percentage
        float perc = (*this->ease)(this->currentTime / this->totalTime);

        switch (getStyleValueType(this->name)) {
            case StyleValueType::INTEGER: {
                int a = std::stoi(this->startValue);
                int b = std::stoi(this->endValue);
                int val = (int)(std::round(a * (1 - perc) + b * perc));
                return std::to_string(val);
            }
            case StyleValueType::EXPRESSION: {
                return std::format("({}) * {} + ({}) * {}", startValue, 1 - perc, endValue, perc);
            }
            case StyleValueType::COLOR: {
                corn::Color c1 = corn::Color::parse(this->startValue);
                corn::Color c2 = corn::Color::parse(this->endValue);
                auto [h1, s1, l1, a1] = c1.getHSLA();
                auto [h2, s2, l2, a2] = c2.getHSLA();
                // Interpolate degree
                corn::Deg h;
                if ((h2 - h1).get() <= 180.0f) {
                    h = h1 + (h2 - h1) * perc;
                } else {
                    h = h1 - (h1 - h2) * perc;
                }
                float s = s1 * (1 - perc) + s2 * perc;
                float l = l1 * (1 - perc) + l2 * perc;
                auto a = (unsigned char)((float)a1 * (1 - perc) + (float)a2 * perc);
                return corn::Color::hsl({ h, s, l, a }).hexStringAlpha();
            }
            default:
                return this->endValue;
        }
    }

    DOMNode::DOMNode() noexcept : dom_(nullptr), parent_(nullptr), widgetID_(0) {}

    DOMNode::~DOMNode() {
        this->clear();
    }

    void DOMNode::clear() noexcept {
        this->clearChildren();
        this->tag_ = "";
        this->name_ = "";
        this->text_ = u8"";
        this->classList_.clear();
        this->style_.clear();
        this->inheritedStyle_.clear();
        this->computedStyle_.clear();
        this->attributes_.clear();
        this->parent_ = nullptr;
        this->children_.clear();
        this->widgetID_ = 0;
    }

    void DOMNode::clearChildren() noexcept {
        for (DOMNode* child : this->children_) {
            child->clear();
            delete child;
        }
        this->children_.clear();
    }

    DOMNode::DOMNode(const DOMNode& other) noexcept : DOMNode() {
        this->tag_ = other.tag_;
        this->name_ = other.name_;
        this->text_ = other.text_;
        this->classList_ = other.classList_;
        this->style_ = other.style_;
        this->attributes_ = other.attributes_;
        this->dom_ = other.dom_;
        this->parent_ = nullptr;
        for (const DOMNode* child : other.children_) {
            auto* childCopy = new DOMNode(*child);
            childCopy->parent_ = this;
            this->children_.push_back(childCopy);
        }
    }

    DOMNode& DOMNode::operator=(const DOMNode& other) noexcept {
        if (&other == this) return *this;
        this->clear();
        this->tag_ = other.tag_;
        this->name_ = other.name_;
        this->text_ = other.text_;
        this->classList_ = other.classList_;
        this->style_ = other.style_;
        this->attributes_ = other.attributes_;
        this->dom_ = other.dom_;
        this->parent_ = nullptr;
        for (const DOMNode* child : other.children_) {
            auto* childCopy = new DOMNode(*child);
            childCopy->parent_ = this;
            this->children_.push_back(childCopy);
        }
        this->widgetID_ = 0;
        return *this;
    }

    bool DOMNode::hasClass(const std::string& className) const noexcept {
        return std::find(this->classList_.begin(), this->classList_.end(), className) == this->classList_.end();
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

    void DOMNode::sync() {
        // Syncing a text node is equivalent to syncing its parent
        if (this->tag_ == "text") {
            if (this->parent_) this->parent_->sync();
            return;
        }

        // Must be bound to a UI manager
        if (!this->dom_ || !this->dom_->getUIManager()) return;
        corn::UIManager& uiManager = *this->dom_->getUIManager();

        // Root node only syncs its children
        if (!this->parent_) {
            this->syncChildren();
            return;
        }

        // Get the parent widget and the current widget
        corn::UIWidget* parentWidget = this->parent_->getWidget();
        corn::UIWidget* current = this->getWidget();

        // Check if widget needs to be discarded
        bool needToDiscard =
                current != nullptr &&
                ((current->getType() == corn::UIType::PANEL && this->tag_ != "widget") ||
                 (current->getType() == corn::UIType::LABEL && this->tag_ != "label") ||
                 (current->getType() == corn::UIType::IMAGE && this->tag_ != "image"));
        if (needToDiscard) {
            current->destroy();
            current = nullptr;
        }

        // Create a new widget if needed
        if (current == nullptr) {
            // Recreate the widget
            if (this->tag_ == "widget") {
                current = &uiManager.createWidget<corn::UIWidget>(this->name_, parentWidget);
            } else if (this->tag_ == "label") {
                current = &uiManager.createWidget<corn::UILabel>(this->name_, parentWidget, corn::RichText());
            } else if (this->tag_ == "image") {
                current = &uiManager.createWidget<corn::UIImage>(
                        this->name_, parentWidget,
                        new corn::Image(50, 50, corn::Color::rgb(255, 255, 255, 0)));
            } else {
                // Invalid tag
                return;
            }

            // Update widgetIDs
            this->widgetID_ = current->getID();
            for (DOMNode* child: this->children_) {
                child->widgetID_ = 0;
            }

            // Register scripts as callbacks
            current->getEventManager().addListener(
                    "corn::ui::keyboard",
                    [this](const corn::EventArgs& args) {
                        const auto& args_ = dynamic_cast<const corn::EventArgsUIKeyboard&>(args);
                        switch (args_.keyboardEvent.status) {
                            case corn::ButtonEvent::DOWN:
                                this->runScriptInAttr("onkeydown", args_.keyboardEvent.key);
                                break;
                            case corn::ButtonEvent::UP:
                                this->runScriptInAttr("onkeyup", args_.keyboardEvent.key);
                                break;
                        }
                    });
            current->getEventManager().addListener(
                    "corn::input::text",
                    [this](const corn::EventArgs& args) {
                        const auto& args_ = dynamic_cast<const corn::EventArgsTextEntered&>(args);
                        this->runScriptInAttr("ontext", args_.character);
                    });
            current->getEventManager().addListener(
                    "corn::ui::onclick",
                    [this, current](const corn::EventArgs& args) {
                        const auto& args_ = dynamic_cast<const corn::EventArgsUIOnClick&>(args);
                        if (args_.target == current && args_.mousebtnEvent.status == corn::ButtonEvent::UP) {
                            this->runScriptInAttr("onclick");
                        }
                    });
            current->getEventManager().addListener(
                    "corn::ui::onhover",
                    [this](const corn::EventArgs&) {
                        this->runScriptInAttr("onhover");
                    });
            current->getEventManager().addListener(
                    "corn::ui::onenter",
                    [this](const corn::EventArgs&) {
                        this->runScriptInAttr("onenter");
                    });
            current->getEventManager().addListener(
                    "corn::ui::onexit",
                    [this](const corn::EventArgs&) {
                        this->runScriptInAttr("onexit");
                    });
            current->getEventManager().addListener(
                    "corn::ui::onscroll",
                    [this, current](const corn::EventArgs& args) {
                        const auto& args_ = dynamic_cast<const corn::EventArgsUIOnScroll&>(args);
                        if (args_.target == current) {
                            this->runScriptInAttr("onscroll", args_.mousescEvent.value);
                        }
                    });
            current->getEventManager().addListener(
                    "corn::ui::onfocus",
                    [this](const corn::EventArgs&) {
                        this->runScriptInAttr("onfocus");
                    });
            current->getEventManager().addListener(
                    "corn::ui::onunfocus",
                    [this](const corn::EventArgs&) {
                        this->runScriptInAttr("onunfocus");
                    });
        }

        // Apply styles
        if (this->widgetID_ > 0) {
            corn::UIWidget* widget = this->dom_->getUIManager()->getWidgetByID(this->widgetID_);
            if (widget != nullptr) {
                // Apply general styles
                widget->setName(this->name_);
                if (this->computedStyle_.at("active") == "true") {
                    widget->setActive(true);
                } else if (this->computedStyle_.at("active") == "false") {
                    widget->setActive(false);
                }
                widget->setX(this->computedStyle_.at("x"));
                widget->setY(this->computedStyle_.at("y"));
                widget->setW(this->computedStyle_.at("w"));
                widget->setH(this->computedStyle_.at("h"));
                widget->setZOrder(std::stoi(this->computedStyle_.at("z-order")));
                if (this->computedStyle_.at("keyboard-interactable") == "true") {
                    widget->setKeyboardInteractable(true);
                } else if (this->computedStyle_.at("keyboard-interactable") == "false") {
                    widget->setKeyboardInteractable(false);
                }
                if (this->computedStyle_.at("mouse-interactable") == "true") {
                    widget->setMouseInteractable(true);
                } else if (this->computedStyle_.at("mouse-interactable") == "false") {
                    widget->setMouseInteractable(false);
                }
                if (this->computedStyle_.at("overflow") == "display") {
                    widget->setOverflow(corn::UIOverflow::DISPLAY);
                } else if (this->computedStyle_.at("overflow") == "hidden") {
                    widget->setOverflow(corn::UIOverflow::HIDDEN);
                }
                widget->setBackground(corn::Color::parse(this->computedStyle_.at("background")));
                widget->setOpacity((unsigned char)std::stoi(this->computedStyle_.at("opacity")));

                // Apply widget type-specific styles
                if (this->tag_ == "label") {
                    ((corn::UILabel*) widget)->setText(this->getRichText());
                } else if (this->tag_ == "image") {
                    auto* image = new corn::Image(this->dom_->getFile().parent_path() / this->attributes_.at("src"));
                    ((corn::UIImage*)widget)->setImage(image);
                }
            }
        }

        // Sync children
        this->syncChildren();
    }

    bool DOMNode::animate(const std::string& name, const std::string& value, std::unique_ptr<EasingFunction> ease, float duration) noexcept {
        switch (getStyleValueType(name)) {
            case StyleValueType::NONE:
                return false;
            case StyleValueType::INTEGER:
            case StyleValueType::EXPRESSION:
            case StyleValueType::COLOR:
                // Invalid duration
                if (duration <= 0.0f) {
                    this->setStyle(name, value);
                    return false;
                }

                // Stop previous animation
                if (this->animations_.contains(name)) {
                    // If the animation is already running, update the end value
                    this->setStyle(name, this->animations_.at(name).endValue);
                    this->animations_.erase(name);
                }

                // Start the animation
                this->animations_[name] = {
                        name, this->computedStyle_.at(name), value, std::move(ease), duration, 0.0f
                };
                this->getDOM()->getUIManager()->getScene().getEventManager().emit(EventArgsAnimation(this));

                return true;
            default:
                this->setStyle(name, value);
                return false;
        }
    }

    void DOMNode::syncChildren() const {
        for (DOMNode* child : this->children_) {
            // Skip text nodes
            if (child->tag_ == "text") continue;
            child->sync();
        }
    }

    void DOMNode::computeElementStyle() {
        // Reset computedStyles to default
        this->computedStyle_ = {
                { "active", "true" },
                { "x", "0px" }, { "y", "0px" }, { "w", "100%nw" }, { "h", "100%nh" },
                { "z-order", "0" },
                { "keyboard-interactable", "false" },
                { "mouse-interactable", "false" },
                { "overflow", "display" },
                { "background", "#ffffff00" },
                { "opacity", "255" },
                { "font-family", "noto-sans" },
                { "font-size", "16" },
                { "font-color", "#000000" },
                { "font-variant", "regular" }
        };

        // Inherit
        for (const auto& [name, value] : this->inheritedStyle_) {
            this->computedStyle_[name] = value;
        }

        // Styles from stylesheet
        for (const CSSRule& style : this->getDOM()->getCSSOM().getRules()) {
            // Apply the styles if the selector matches the current node
            if (match(style.selector, *this)) {
                for (const auto& [name, value] : style.declarations) {
                    this->computedStyle_[name] = value;
                }
            }
        }

        // Inline styles
        for (const auto& [name, value] : this->style_) {
            this->computedStyle_[name] = value;
        }

        // Compute styles of child nodes
        for (DOMNode* child : this->children_) {
            child->computeStyle(this->computedStyle_);
        }
    }

    void DOMNode::computeTextStyle() {
        // Only compute font-related styles
        this->computedStyle_ = {
                { "font-family", "noto-sans" },
                { "font-size", "16" },
                { "font-color", "#000000" },
                { "font-variant", "regular" }
        };

        // Inherit
        for (const auto& [name, value] : this->inheritedStyle_) {
            this->computedStyle_[name] = value;
        }

        // Styles from stylesheet
        for (const CSSRule& style : this->getDOM()->getCSSOM().getRules()) {
            // Apply the styles if the selector matches the current node
            if (match(style.selector, *this)) {
                for (const auto& [name, value] : style.declarations) {
                    this->computedStyle_[name] = value;
                }
            }
        }

        // Inline styles
        for (const auto& [name, value] : this->style_) {
            this->computedStyle_[name] = value;
        }

        // No need to apply styles since text nodes do not have widgets

        // Compute styles of child nodes
        for (DOMNode* child : this->children_) {
            child->computeStyle(this->computedStyle_);
        }
    }

    void DOMNode::computeStyle() {
        if (this->tag_ == "text") {
            this->computeTextStyle();
        } else {
            this->computeElementStyle();
        }
    }

    void DOMNode::computeStyle(const std::unordered_map<std::string, std::string>& inheritedStyles) {
        for (const std::string& styleName : { "font-family", "font-size", "font-color", "font-variant" }) {
            if (inheritedStyles.contains(styleName)) {
                this->inheritedStyle_[styleName] = inheritedStyles.at(styleName);
            }
        }
        this->computeStyle();
    }

    void DOMNode::runScriptInAttr(const std::string& attr) {
        if (!this->attributes_.contains(attr)) return;
        duk_context* ctx = this->dom_->getUI().getJSRuntime()->getImpl()->ctx_;

        // Compile and run the function stored in the attribute
        const std::string& jsCode = this->attributes_.at(attr);
        if (duk_pcompile_string(ctx, 0, jsCode.c_str()) != 0) {
            fprintf(stderr, "Error compiling JS script: %s\n%s\n", jsCode.c_str(), duk_safe_to_string(ctx, -1));
        } else {
            // Push the "this" value onto the stack
            push_domNode(ctx, this);
            // Call the function
            duk_pcall_method(ctx, 0);
        }

        // Pop the result or error
        duk_pop(ctx);
    }

    void DOMNode::runScriptInAttr(const std::string& attr, float value) {
        if (!this->attributes_.contains(attr)) return;
        duk_context* ctx = this->dom_->getUI().getJSRuntime()->getImpl()->ctx_;

        // Set the value
        duk_push_number(ctx, value);
        duk_put_global_string(ctx, "__value");

        // Run the script
        this->runScriptInAttr(attr);
    }

    void DOMNode::runScriptInAttr(const std::string& attr, const corn::Key& key) {
        if (!this->attributes_.contains(attr)) return;
        duk_context* ctx = this->dom_->getUI().getJSRuntime()->getImpl()->ctx_;

        // Set the key
        std::string keyStr = toString(key);
        duk_push_string(ctx, keyStr.c_str());
        duk_put_global_string(ctx, "__key");

        // Run the script
        this->runScriptInAttr(attr);
    }

    void DOMNode::runScriptInAttr(const std::string& attr, const std::u8string& text) {
        if (!this->attributes_.contains(attr)) return;
        duk_context* ctx = this->dom_->getUI().getJSRuntime()->getImpl()->ctx_;

        // Set the text
        duk_push_string(ctx, (const char*)text.c_str());
        duk_put_global_string(ctx, "__text");

        // Run the script
        this->runScriptInAttr(attr);
    }
}
