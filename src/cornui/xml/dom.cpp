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

        // Sync with the UI
        this->root_.sync();
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

    std::unordered_map<std::string, DOM::Def>& DOM::getDefs() noexcept {
        return this->defs_;
    }

    const std::unordered_map<std::string, DOM::Def>& DOM::getDefs() const noexcept {
        return this->defs_;
    }

    corn::UIManager* DOM::getUIManager() noexcept {
        return this->uiManager_;
    }

    const corn::UIManager* DOM::getUIManager() const noexcept {
        return this->uiManager_;
    }
}
