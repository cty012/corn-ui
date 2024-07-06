#pragma once

#include <filesystem>
#include <string>
#include <cornui/css/cssom.h>
#include <cornui/xml/dom_node.h>

namespace corn {
    class UIManager;
}

namespace cornui {
    class UI;

    /**
     * @class DOM
     * @brief A tree structure that represent the DOM of the front end. Note this class is only intended to store data.
     * It is not linked to the actual UI on the screen.
     */
    class DOM {
    public:
        friend class UI;

        struct Def {
            std::string tag;
            std::filesystem::path file;
            DOMNode node;
        };

        /**
         * @brief Bind the data in the DOM tree with the UI manager. Changes in the DOM will sync in the UI.
         * @param uiManager The UI manager for generating the UI.
         * @param dom The DOM tree containing the layout.
         *
         * DO NOT alter the UI manager after binding!!
         */
        void bind(corn::UIManager& uiManager);

        /**
         * @param pred A predicate function that takes a DOM node pointer and returns whether it satisfy the conditions.
         * @param parent Parent to start searching from.
         * @return All DOM nodes that satisfy the conditions given by the predicate function, or null pointer if it
         *         doesn't exist.
         */
        [[nodiscard]] DOMNode* getNodeThat(
                const std::function<bool(const DOMNode* node)>& pred, const DOMNode* parent = nullptr) const;

        /**
         * @param pred A predicate function that takes a DOM node pointer and returns whether it satisfy the conditions.
         * @param parent Parent to start searching from.
         * @return All DOM nodes that satisfy the conditions given by the predicate function, or null pointer if it
         *         doesn't exist.
         */
        [[nodiscard]] std::vector<DOMNode*> getNodesThat(
                const std::function<bool(const DOMNode* node)>& pred, const DOMNode* parent = nullptr) const;

        /**
         * @param parent Parent to start searching from.
         * @return All DOM nodes.
         */
        [[nodiscard]] std::vector<DOMNode*> getAllNodes(const DOMNode* parent = nullptr) const;

        /**
         * @brief Get any node that matches the given CSS selector.
         * @param selector The CSS selector to match.
         * @param parent Parent to start searching from.
         * @return A node that matches the selector.
         */
        [[nodiscard]] DOMNode* getNodeBySelector(const CSSSelector& selector, const DOMNode* parent = nullptr) const;

        /**
         * @brief Get all nodes that match the given CSS selector.
         * @param selector The CSS selector to match.
         * @param parent Parent to start searching from.
         * @return All nodes that match the selector.
         */
        [[nodiscard]] std::vector<DOMNode*> getNodesBySelector(
                const CSSSelector& selector, const DOMNode* parent = nullptr) const;

        /**
         * @brief Get any node that matches the given CSS selector.
         * @param selector The CSS selector to match.
         * @param parent Parent to start searching from.
         * @return A node that matches the selector.
         */
        [[nodiscard]] DOMNode* getNodeBySelector(const std::string& selector, const DOMNode* parent = nullptr) const;

        /**
         * @brief Get all nodes that match the given CSS selector.
         * @param selector The CSS selector to match.
         * @param parent Parent to start searching from.
         * @return All nodes that match the selector.
         */
        [[nodiscard]] std::vector<DOMNode*> getNodesBySelector(
                const std::string& selector, const DOMNode* parent = nullptr) const;

        [[nodiscard]] const UI& getUI() const noexcept;
        [[nodiscard]] UI& getUI() noexcept;
        [[nodiscard]] const std::filesystem::path& getFile() const noexcept;
        [[nodiscard]] DOMNode& getRoot() noexcept;
        [[nodiscard]] const DOMNode& getRoot() const noexcept;
        [[nodiscard]] CSSOM& getCSSOM() noexcept;
        [[nodiscard]] const CSSOM& getCSSOM() const noexcept;
        [[nodiscard]] std::unordered_map<std::string, Def>& getDefs() noexcept;
        [[nodiscard]] const std::unordered_map<std::string, Def>& getDefs() const noexcept;
        [[nodiscard]] corn::UIManager* getUIManager() noexcept;
        [[nodiscard]] const corn::UIManager* getUIManager() const noexcept;

    private:
        /// @brief Empty constructor.
        explicit DOM(UI* ui);
        DOM(const DOM& dom) = delete;
        DOM& operator=(const DOM& dom) = delete;

        /**
         * @brief Constructor. The DOM tree MUST be loaded from a XML file.
         * @param file The path to the XML file.
         * @param jsList List of JavaScript files to load.
         * @throw std::invalid_argument If file cannot be opened, or the contents of the file cannot be parsed.
         */
        void init(const std::filesystem::path& file, std::vector<std::filesystem::path>& jsList);

        UI* ui_;
        std::filesystem::path file_;
        DOMNode root_;
        std::unordered_map<std::string, Def> defs_;
        CSSOM cssom_;
        corn::UIManager* uiManager_;
    };
}
