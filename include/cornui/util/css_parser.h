#pragma once

#include <istream>
#include <string>
#include <unordered_map>
#include <vector>

namespace cornui {
    struct CSSRule;

    bool isValidIdentifier(const std::string& token) noexcept;

    std::unordered_map<std::string, std::string> parseDeclFromString(const std::string& contents);

    std::vector<CSSRule> parseCSSFromStream(std::istream& contents);
}
