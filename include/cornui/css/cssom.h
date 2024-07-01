#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

namespace cornui {
    enum class CSSSelectorCombinator {
        DESCENDANT, CHILD, ADJACENT_SIBLING, GENERAL_SIBLING, GROUP, NONE
    };

    struct CSSBasicSelector {
        std::vector<std::string> parts;
    };

    /**
     * @class CSSSelectorGroup
     * @brief A group within a CSS selector, separated by commas.
     */
    struct CSSSelectorGroup {
        std::vector<CSSBasicSelector> basicSelectors;
        std::vector<CSSSelectorCombinator> combinators;
    };

    /**
     * @class CSSSelector
     * @brief CSS selector as defined by W3C.
     */
    struct CSSSelector {
        std::vector<CSSSelectorGroup> groups;
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

        CSSOM(const CSSOM& other) = delete;
        CSSOM& operator=(const CSSOM& other) = delete;

        /**
         * @brief Parse CSS from a given string.
         * @param contents The string to parse.
         */
        void loadFromStream(std::istream& input);

        /**
         * @brief Parse CSS from a given string.
         * @param contents The string to parse.
         */
        void loadFromString(const std::string& contents);

        /**
         * @brief Load CSS from a file.
         * @param file The path to the CSS file.
         */
        void loadFromFile(const std::filesystem::path& file);

        [[nodiscard]] const std::vector<CSSRule>& getRules() const noexcept;

        /**
         * @brief Add a new rule to the CSSOM.
         * @param rule The rule to add. Will be checked against existing rules when merging.
         */
        void addRule(const CSSRule& rule);

    private:
        std::vector<CSSRule> rules_;
    };

    bool precede(const CSSSelectorGroup& selector1, const CSSSelectorGroup& selector2);

    class DOMNode;
    /**
     * @param selector The CSS selector.
     * @param node The node to match.
     * @param root The root node specifying the scope. Null pointer refers to the highest root.
     * @return Whether it is a match.
     */
    bool match(const CSSSelector& selector, const DOMNode& node, const DOMNode* root = nullptr);
}
