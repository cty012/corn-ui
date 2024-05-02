#pragma once

#include <istream>
#include <string>
#include <unordered_map>
#include <vector>

namespace cornui {
    enum class CSSSelectorCombinator;
    struct CSSSelectorGroup;
    struct CSSSelector;
    struct CSSRule;

    bool isValidIdentifier(const std::string& token) noexcept;

    CSSSelectorCombinator parseCombinatorFromChar(char ch) noexcept;

    CSSSelector parseSelectorFromString(const std::string& contents);

    std::unordered_map<std::string, std::string> parseDeclFromString(const std::string& contents);

    std::vector<CSSRule> parseCSSFromStream(std::istream& contents);
}
