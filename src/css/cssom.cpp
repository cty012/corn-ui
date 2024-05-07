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
        // Groups are stored separately due to precedence rules
        for (const CSSSelectorGroup& group : rule.selector.groups) {
            this->rules_.push_back(CSSRule{
                CSSSelector{ std::vector<CSSSelectorGroup>{ group } },
                rule.declarations
            });
        }

        // Sort by precedence
        std::ranges::stable_sort(this->rules_, [](const CSSRule& first, const CSSRule& second) {
            return !precede(first.selector.groups[0], second.selector.groups[0]);
        });
    }

    std::tuple<size_t, size_t, size_t> countSelectorLevel(const CSSSelectorGroup& selector) {
        size_t lvl1 = 0, lvl2 = 0, lvl3 = 0;
        for (const CSSBasicSelector& basicSelector : selector.basicSelectors) {
            for (const std::string& part : basicSelector.parts) {
                if (part.empty()) continue;
                else if (part[0] == '#') lvl1++;
                else if (part[1] == '.') lvl2++;
                else lvl3++;
            }
        }
        return { lvl1, lvl2, lvl3 };
    }

    bool precede(const CSSSelectorGroup& selector1, const CSSSelectorGroup& selector2) {
        // Level 1: Name selector
        // Level 2: Class selector
        // Level 3: Tag selector
        auto [s1Lvl1, s1Lvl2, s1Lvl3] = countSelectorLevel(selector1);
        auto [s2Lvl1, s2Lvl2, s2Lvl3] = countSelectorLevel(selector2);
        if (s1Lvl1 != s2Lvl1) return s1Lvl1 > s2Lvl1;
        if (s1Lvl2 != s2Lvl2) return s1Lvl2 > s2Lvl2;
        if (s1Lvl3 != s2Lvl3) return s1Lvl3 > s2Lvl3;
        return false;
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
