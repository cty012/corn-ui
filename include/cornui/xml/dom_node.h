#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <corn/ui.h>

namespace cornui {
    class DOM;

    class DOMNode {
    public:
        friend class DOM;

        DOMNode() noexcept;
        ~DOMNode();

        /**
         * Clears all descendants of the node.
         * @param node The target node to clear.
         */
        void clear() noexcept;

        /**
         * @brief Copy constructor & Copy assignment operator.
         *
         * Note: Descendants will be copied and attached. The parent_ property will point to null point (target node
         * only).
         */
        DOMNode(const DOMNode& other) noexcept;
        DOMNode& operator=(const DOMNode& other) noexcept;

        /**
         * @param node The target node.
         * @return The inner XML as a string.
         */
        [[nodiscard]] std::string getInnerXML() const noexcept;

        /**
         * @param node The target node.
         * @return The inner XML as a string.
         */
        [[nodiscard]] std::string getOuterXML() const noexcept;

        /**
         * @param className The class to check.
         * @return Whether the DOM node has the class.
         */
        [[nodiscard]] bool hasClass(const std::string& className) const noexcept;

        /**
         * @brief Add a class to the node.
         * @param className The class to add.
         * @return Whether the class is successfully added. Only true if class does not already exist and has correct
         * style.
         */
        bool addClass(const std::string& className) noexcept;

        /**
         * @brief Remove a class from the node.
         * @param className The class to remove
         * @return Whether the class is successfully removed. Only true if the node previously has this class.
         */
        bool removeClass(const std::string& className) noexcept;

        // Getters & Setters
        [[nodiscard]] const std::string& getTag() const noexcept;
        [[nodiscard]] const std::string& getName() const noexcept;
        void setName(const std::string& name) noexcept;
        [[nodiscard]] const std::u8string& getText() const noexcept;
        void setText(const std::u8string& text) noexcept;
        [[nodiscard]] const std::vector<std::string>& getClassList() const noexcept;
        [[nodiscard]] const std::unordered_map<std::string, std::string>& getStyle() const noexcept;
        void setStyle(const std::string& name, const std::string& value) noexcept;
        [[nodiscard]] const std::unordered_map<std::string, std::string>& getComputedStyle() const noexcept;
        [[nodiscard]] const std::unordered_map<std::string, std::string>& getAttributes() const noexcept;
        void setAttribute(const std::string& name, const std::string& value) noexcept;
        void removeAttribute(const std::string& name) noexcept;

        [[nodiscard]] DOM* getDOM() const noexcept;
        [[nodiscard]] DOMNode* getParent() const noexcept;
        [[nodiscard]] const std::vector<DOMNode*>& getChildren() const noexcept;
        [[nodiscard]] corn::UIWidget::WidgetID getWidgetID() const noexcept;
        [[nodiscard]] corn::UIWidget* getWidget() const noexcept;

    private:
        void computeStyle();
        void computeStyle(const std::unordered_map<std::string, std::string>& inheritedStyles);

        /**
         * @brief Executes the JavaScript string specified with the given attribute.
         * @param attr Name of the attribute.
         *
         * This function is automatically triggered by corresponding events.
         */
        void runScriptInAttr(const std::string& attr);

        /**
         * @brief Executes the JavaScript string specified with the given attribute.
         * @param attr Name of the attribute.
         * @param key The key triggered.
         *
         * This function is automatically triggered by corresponding events.
         */
        void runScriptInAttr(const std::string& attr, const corn::Key& key);

        /**
         * @brief Executes the JavaScript string specified with the given attribute.
         * @param attr Name of the attribute.
         * @param text The text entered.
         *
         * This function is automatically triggered by corresponding events.
         */
        void runScriptInAttr(const std::string& attr, const std::u8string& text);

        std::string tag_;
        std::string name_;
        std::u8string text_;
        std::vector<std::string> classList_;
        std::unordered_map<std::string, std::string> style_;
        std::unordered_map<std::string, std::string> inheritedStyle_;
        std::unordered_map<std::string, std::string> computedStyle_;
        std::unordered_map<std::string, std::string> attributes_;

        DOM* dom_;
        DOMNode* parent_;
        std::vector<DOMNode*> children_;
        corn::UIWidget::WidgetID widgetID_;
    };
}
