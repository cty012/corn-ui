#include <regex>
#include <sstream>
#include <corn/util/string_utils.h>
#include <cornui/css/cssom.h>
#include <cornui/util/css_parser.h>
#include <cornui/util/exception.h>

namespace cornui {
    bool isValidIdentifier(const std::string& token) noexcept {
        static const std::regex pattern("^[a-zA-Z_-][a-zA-Z0-9_-]*$");
        return std::regex_match(token, pattern);
    }

    /**
     * @brief Helper function. Get string from stream until reaching the delimiter.
     * @param stream The input stream to read from.
     * @param token The string to write to.
     * @param delim The delimiter.
     * @return Whether the delimiter is reached.
     */
    bool getStringUntilDelim(std::istream& stream, std::string& token, char delim) {
        char currentChar;
        bool inSingleLineComment = false, inMultiLineComment = false;
        token.clear();

        while (stream.get(currentChar)) {
            if (inSingleLineComment) {
                // Check for the end of single-line comment
                if (currentChar == '\n') inSingleLineComment = false;
            } else if (inMultiLineComment) {
                // Check for the end of multi-line comment
                if (currentChar == '*' && stream.peek() == '/') {
                    inMultiLineComment = false;
                    stream.get();  // Skip the '/' character
                    continue;
                }
            } else {
                // Not in comment
                if (currentChar == '/' && stream.peek() == '/') {
                    // Check for the start of single-line comment
                    inSingleLineComment = true;
                } else if (currentChar == '/' && stream.peek() == '*') {
                    // Check for the start of multi-line comment
                    inMultiLineComment = true;
                    stream.get();  // Skip the '*' character
                } else if (currentChar == delim) {
                    // Found the target character outside of a comment
                    return true;
                } else {
                    // Non-target character outside of comment
                    token += currentChar;
                }
            }
        }

        return false;
    }

    std::unordered_map<std::string, std::string> parseDeclFromString(const std::string& contents) {
        std::istringstream stream(contents);
        std::string token;
        std::unordered_map<std::string, std::string> declarations;
        while (true) {
            // Get the key
            bool hasColon = getStringUntilDelim(stream, token, ':');
            std::string untrimmed = token + ":";
            std::string key = corn::trim(token);
            if (!hasColon) {
                if (key.empty()) {
                    return declarations;
                } else {
                    throw CSSDeclSyntaxError(key, "Delimiter \":\" is expected.");
                }
            } else {
                if (!isValidIdentifier(key)) {
                    throw CSSDeclSyntaxError(
                            key, "Identifier must contain only letters, digits, underscore, or hyphen, and cannot start with a digit.");
                }
            }

            // Get the value
            bool hasSemicolon = getStringUntilDelim(stream, token, ';');
            untrimmed += token;
            std::string value = corn::trim(token);
            if (!hasSemicolon) {
                throw CSSDeclSyntaxError(corn::trim(untrimmed), "Expect token \";\" at the end of declaration.");
            }

            // Add to the results
            declarations[key] = value;
        }
    }

    std::vector<CSSRule> parseCSSFromStream(std::istream& contents) {
        // @todo: Be more robust against errors in the CSS string.
        std::string token;
        std::vector<CSSRule> result;

        while (true) {
            // Get the selector
            bool hasLeftBracket = getStringUntilDelim(contents, token, '{');
            std::string untrimmed = token + "{";
            std::string selector = corn::trim(token);
            if (!hasLeftBracket) {
                if (selector.empty()) {
                    return result;
                } else {
                    throw CSSRuleSyntaxError(selector, "Expect token \"{\" after the selector.");
                }
            }

            // @todo Parse selector.

            // Get the declarations
            bool hasRightBracket = getStringUntilDelim(contents, token, '}');
            untrimmed += token;
            std::string declString = corn::trim(token);
            if (!hasRightBracket) {
                throw CSSRuleSyntaxError(corn::trim(untrimmed), "Expect token \"}\" at the end of rule.");
            }
            if (token.find('{') != std::string::npos) {
                throw CSSRuleSyntaxError(corn::trim(untrimmed) + "}", "Nested brackets are not allowed.");
            }

            // Add to the results
            result.push_back({ selector, parseDeclFromString(declString) });
        }
    }
}
