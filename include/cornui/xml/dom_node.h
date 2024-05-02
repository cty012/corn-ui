#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <corn/ui.h>

namespace cornui {
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

        // Copy constructor & Copy assignment operator
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

        // Getters & Setters
        [[nodiscard]] const std::string& getTag() const noexcept;
        [[nodiscard]] const std::string& getName() const noexcept;
        void setName(const std::string& name) noexcept;
        [[nodiscard]] const std::u8string& getText() const noexcept;
        void setText(const std::u8string& text) noexcept;
        [[nodiscard]] const std::unordered_map<std::string, std::string>& getStyles() const noexcept;
        void setStyle(const std::string& name, const std::string& value) noexcept;
        [[nodiscard]] const std::unordered_map<std::string, std::string>& getComputedStyles() const noexcept;
        [[nodiscard]] const std::unordered_map<std::string, std::string>& getAttributes() const noexcept;
        void setAttributes(const std::string& name, const std::string& value) noexcept;
        [[nodiscard]] const std::vector<DOMNode*>& getChildren() const noexcept;

    private:
        void computeStyle();
        void computeStyle(const std::unordered_map<std::string, std::string>& inheritedStyles);

        std::string tag_;
        std::string name_;
        std::u8string text_;
        std::vector<std::string> classList_;
        std::unordered_map<std::string, std::string> styles_;
        std::unordered_map<std::string, std::string> inheritedStyles_;
        std::unordered_map<std::string, std::string> computedStyles_;
        std::unordered_map<std::string, std::string> attributes_;
        std::vector<DOMNode*> children_;
        corn::UIWidget::WidgetID widgetID_;
    };
}
