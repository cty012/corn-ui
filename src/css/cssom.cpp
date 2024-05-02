#include <algorithm>
#include <fstream>
#include <ranges>
#include <sstream>
#include <vector>
#include <corn/util/string_utils.h>
#include <cornui/css/cssom.h>
#include <cornui/util/css_parser.h>
#include <cornui/xml/dom.h>

namespace cornui {
    CSSOM::CSSOM() noexcept = default;

    CSSOM::~CSSOM() = default;

    CSSOM& CSSOM::instance() noexcept {
        static CSSOM singleton;
        return singleton;
    }

    void CSSOM::loadFromStream(std::istream& input) {
        for (const CSSRule& rule : parseCSSFromStream(input)) {
            this->addRule(rule);
        }
    }

    void CSSOM::loadFromString(const std::string& contents) {
        std::istringstream input(contents);
        this->loadFromStream(input);
    }

    void CSSOM::loadFromFile(const std::filesystem::path& file) {
        std::ifstream input(file);
        this->loadFromStream(input);
    }

    const std::vector<CSSRule>& CSSOM::getRules() const noexcept {
        return this->rules_;
    }

    void CSSOM::addRule(const CSSRule& rule) {
        // Currently no reduction is applied
        this->rules_.push_back(rule);
    }

    // Helper
    bool matchBasicSelector(const CSSBasicSelector& basicSelector, const DOMNode& node) {
        const std::vector<std::string>& classes = node.getClassList();

        // All parts should match
        return std::ranges::all_of(basicSelector.parts, [&node, &classes](const std::string& part) {
            if (part.empty()) return true;
            if (part[0] == '.') { // Class selector
                std::string expectedClass = part.substr(1);
                return std::find(classes.begin(), classes.end(), expectedClass) != classes.end();
            } else if (part[0] == '#') { // Name selector
                return part.substr(1) == node.getName();
            } else { // Tag selector
                return part == node.getTag();
            }
        });
    }

    bool matchGroup(const CSSSelectorGroup& selectorGroup, const DOMNode& node) {
        const std::vector<CSSBasicSelector>& basicSelectors = selectorGroup.basicSelectors;
        const std::vector<CSSSelectorCombinator>& combinators = selectorGroup.combinators;

        if (basicSelectors.empty()) return false;
        int index = (int)basicSelectors.size() - 1;
        const DOMNode* currentNode = &node;

        // Match target node first
        if (!matchBasicSelector(basicSelectors.at(index), *currentNode)) {
            return false;
        }

        // Match ancestors
        while (--index >= 0) {
            // todo: other combinators
            switch (combinators.at(index)) {
                case CSSSelectorCombinator::DESCENDANT:
                    // Find any ancestor that matches the next selector
                    do {
                        currentNode = currentNode->getParent();
                        if (!currentNode) return false;
                    } while (!matchBasicSelector(basicSelectors.at(index), *currentNode));
                    break;
                case CSSSelectorCombinator::CHILD:
                    currentNode = currentNode->getParent();
                    if (!currentNode || !matchBasicSelector(basicSelectors.at(index), *currentNode)) {
                        return false;
                    }
                    break;
                default:
                    return false; // Unsupported combinator
            }
        }

        return index < 0; // Match successful if all selectors matched
    }

    bool match(const CSSSelector& selector, const DOMNode& node) {
        return std::ranges::any_of(selector.groups, [&node](const CSSSelectorGroup& group) {
            return matchGroup(group, node);
        });
    }
}
