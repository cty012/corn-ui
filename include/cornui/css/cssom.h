#pragma once

#include <string>
#include <unordered_map>

namespace cornui {
    /**
     * @class CSSSelector
     * @brief CSS selector as defined by W3C.
     */
    struct CSSSelector {
        std::string str;
    };

    /**
     * @class CSSRule
     * @brief A rule that applies a list of declarations to a selector.
     */
    struct CSSRule {
        CSSSelector selector;
        std::unordered_map<std::string, std::string> declarations;
    };

    /**
     * @class CSSOM
     * @brief The CSS Object Model. Stored in files with extension ".ccss", which stands for "Corn CSS".
     */
    class CSSOM {
    public:
        /// @brief Constructor.
        CSSOM() noexcept;

        /// @brief Destructor.
        ~CSSOM();

        /**
         * @brief Parse CSS from a given string.
         * @param contents The string to parse.
         */
        void loadFromString(const std::string& contents);

        /**
         * @brief Load CSS from a file.
         * @param file The path to the CSS file.
         */
        void loadFromFile(const std::string& file);
    };
}
