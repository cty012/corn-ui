#include <queue>
#include <corn/event/event_args.h>
#include <corn/media.h>
#include <corn/ui.h>
#include <corn/util/rich_text.h>
#include <cornui/css/cssom.h>
#include <cornui/util/css_parser.h>
#include <cornui/util/reserved.h>
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

                    // Script attributes
                    current->getEventManager().addListener(
                            "corn::ui::keyboard",
                            [&domNode](const corn::EventArgs& args) {
                                const auto& args_ = dynamic_cast<const corn::EventArgsUIKeyboard&>(args);
                                switch (args_.keyboardEvent.status) {
                                    case corn::ButtonEvent::DOWN:
                                        domNode.runScriptInAttr("onkeydown", args_.keyboardEvent.key);
                                        break;
                                    case corn::ButtonEvent::UP:
                                        domNode.runScriptInAttr("onkeyup", args_.keyboardEvent.key);
                                        break;
                                }
                            });
                    current->getEventManager().addListener(
                            "corn::input::text",
                            [&domNode](const corn::EventArgs& args) {
                                const auto& args_ = dynamic_cast<const corn::EventArgsTextEntered&>(args);
                                domNode.runScriptInAttr("ontext", args_.character);
                            });
                    current->getEventManager().addListener(
                            "corn::ui::onclick",
                            [&domNode, current](const corn::EventArgs& args) {
                                const auto& args_ = dynamic_cast<const corn::EventArgsUIOnClick&>(args);
                                if (args_.target == current && args_.mousebtnEvent.status == corn::ButtonEvent::UP) {
                                    domNode.runScriptInAttr("onclick");
                                }
                            });
                    current->getEventManager().addListener(
                            "corn::ui::onhover",
                            [&domNode](const corn::EventArgs&) {
                                domNode.runScriptInAttr("onhover");
                            });
                    current->getEventManager().addListener(
                            "corn::ui::onenter",
                            [&domNode](const corn::EventArgs&) {
                                domNode.runScriptInAttr("onenter");
                            });
                    current->getEventManager().addListener(
                            "corn::ui::onexit",
                            [&domNode](const corn::EventArgs&) {
                                domNode.runScriptInAttr("onexit");
                            });
                    current->getEventManager().addListener(
                            "corn::ui::onscroll",
                            [&domNode, current](const corn::EventArgs& args) {
                                const auto& args_ = dynamic_cast<const corn::EventArgsUIOnScroll&>(args);
                                if (args_.target == current) {
                                    domNode.runScriptInAttr("onscroll");
                                }
                            });
                    current->getEventManager().addListener(
                            "corn::ui::onfocus",
                            [&domNode](const corn::EventArgs& ) {
                                domNode.runScriptInAttr("onfocus");
                            });
                    current->getEventManager().addListener(
                            "corn::ui::onunfocus",
                            [&domNode](const corn::EventArgs&) {
                                domNode.runScriptInAttr("onunfocus");
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

    DOMNode* DOM::getNodeThat(const std::function<bool(const DOMNode* node)>& pred, const DOMNode* parent) const {
        std::queue<DOMNode*> toCheck;
        if (parent && parent->dom_ != this) return nullptr;
        toCheck.push(const_cast<DOMNode*>(parent ? parent : &this->root_));

        while (!toCheck.empty()) {
            DOMNode* current = toCheck.front();
            toCheck.pop();
            if (!current) continue;

            // If current node is a match, return the current node
            if (pred(current)) {
                return current;
            }

            // Otherwise check the child nodes
            for (DOMNode* child : current->children_) {
                toCheck.push(child);
            }
        }

        return nullptr;
    }

    std::vector<DOMNode*> DOM::getNodesThat(const std::function<bool(const DOMNode* node)>& pred, const DOMNode* parent) const {
        std::vector<DOMNode*> matches;
        std::queue<DOMNode*> toCheck;
        if (parent && parent->dom_ != this) return matches;
        toCheck.push(const_cast<DOMNode*>(parent ? parent : &this->root_));

        while (!toCheck.empty()) {
            DOMNode* current = toCheck.front();
            toCheck.pop();
            if (!current) continue;

            // If current node is a match, add the current node to the list of matches
            if (pred(current)) {
                matches.push_back(current);
            }

            // Otherwise check the child nodes
            for (DOMNode* child : current->children_) {
                toCheck.push(child);
            }
        }

        return matches;
    }

    std::vector<DOMNode*> DOM::getAllNodes(const DOMNode* parent) const {
        return this->getNodesThat([](const DOMNode*) { return true; }, parent);
    }

    DOMNode* DOM::getNodeBySelector(const CSSSelector& selector, const DOMNode* parent) const {
        return this->getNodeThat([&selector](const DOMNode* node){
            return match(selector, *node);
        }, parent);
    }

    std::vector<DOMNode*> DOM::getNodesBySelector(const CSSSelector& selector, const DOMNode* parent) const {
        return this->getNodesThat([&selector](const DOMNode* node){
            return match(selector, *node);
        }, parent);
    }

    DOMNode* DOM::getNodeBySelector(const std::string& selector, const DOMNode* parent) const {
        return this->getNodeBySelector(parseSelectorFromString(selector), parent);
    }

    std::vector<DOMNode*> DOM::getNodesBySelector(const std::string& selector, const DOMNode* parent) const {
        return this->getNodesBySelector(parseSelectorFromString(selector), parent);
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
}
