#include <sstream>
#include <corn/util/string_utils.h>
#include <cornui/css/cssom.h>
#include <cornui/util/css_parser.h>

namespace cornui {
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
            if (!getStringUntilDelim(stream, token, ':')) {
                token = corn::trim(token);
                if (token.empty()) {
                    return declarations;
                } else {
                    // @todo Throw exception saying invalid rule syntax.
                }
            }
            std::string key = corn::trim(token);

            // Get the value
            // @todo Parse selector.
            if (!getStringUntilDelim(stream, token, ';')) {
                // @todo Throw exception saying that expected ';' at the end of a declaration.
            }
            std::string value = corn::trim(token);

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
            if (!getStringUntilDelim(contents, token, '{')) {
                token = corn::trim(token);
                if (token.empty()) {
                    return result;
                } else {
                    // @todo Throw exception saying invalid rule syntax.
                }
            }
            std::string selector = corn::trim(token);

            // Get the declarations
            // @todo Parse selector.
            if (!getStringUntilDelim(contents, token, '}')) {
                // @todo Throw exception saying that expected '}' at the end of a rule.
            }
            if (token.find('{') != std::string::npos) {
                // @todo Throw exception saying that nested brackets are not allowed.
            }
            std::string declString = corn::trim(token);

            // Add to the results
            result.push_back({ selector, parseDeclFromString(declString) });
        }
    }
}
