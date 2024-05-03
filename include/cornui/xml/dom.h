#pragma once

#include <filesystem>
#include <string>
#include <cornui/css/cssom.h>
#include <cornui/xml/dom_node.h>

namespace corn {
    class UIManager;
}

namespace cornui {
    /**
     * @class DOM
     * @brief A tree structure that represent the DOM of the front end. Note this class is only intended to store data.
     * It is not linked to the actual UI on the screen.
     */
    class DOM {
    public:
        /**
         * @brief Constructor. The DOM tree MUST be loaded from a XML file.
         * @param file The path to the XML file.
         * @throw std::invalid_argument If file cannot be opened, or the contents of the file cannot be parsed.
         */
        explicit DOM(std::filesystem::path file);

        DOM(const DOM& dom) = delete;
        DOM& operator=(const DOM& dom) = delete;

        /**
         * @brief Bind the data in the DOM tree with the UI manager. Changes in the DOM will sync in the UI.
         * @param uiManager The UI manager for generating the UI.
         * @param dom The DOM tree containing the layout.
         *
         * DO NOT alter the UI manager after binding!!
         */
        void bind(corn::UIManager& uiManager);

        /**
         * @brief Get any node that matches the given CSS selector.
         * @param selector The CSS selector to match.
         * @return A node that matches the selector.
         */
        [[nodiscard]] DOMNode* getNodeBySelector(const CSSSelector& selector) const;

        /**
         * @brief Get all nodes that match the given CSS selector.
         * @param selector The CSS selector to match.
         * @return All nodes that match the selector.
         */
        [[nodiscard]] std::vector<DOMNode*> getNodesBySelector(const CSSSelector& selector) const;

        /**
         * @brief Get any node that matches the given CSS selector.
         * @param selector The CSS selector to match.
         * @return A node that matches the selector.
         */
        [[nodiscard]] DOMNode* getNodeBySelector(const std::string& selector) const;

        /**
         * @brief Get all nodes that match the given CSS selector.
         * @param selector The CSS selector to match.
         * @return All nodes that match the selector.
         */
        [[nodiscard]] std::vector<DOMNode*> getNodesBySelector(const std::string& selector) const;

        [[nodiscard]] const std::filesystem::path& getFile() const noexcept;
        [[nodiscard]] DOMNode& getRoot() noexcept;
        [[nodiscard]] const DOMNode& getRoot() const noexcept;
        [[nodiscard]] CSSOM& getCSSOM() noexcept;
        [[nodiscard]] const CSSOM& getCSSOM() const noexcept;
        [[nodiscard]] corn::UIManager* getUIManager() noexcept;
        [[nodiscard]] const corn::UIManager* getUIManager() const noexcept;

    private:
        std::filesystem::path file_;
        DOMNode root_;
        CSSOM cssom_;
        corn::UIManager* uiManager_;
    };
}
