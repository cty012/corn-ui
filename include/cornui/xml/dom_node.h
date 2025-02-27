#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <corn/event/input.h>
#include <corn/ui/ui_label.h>
#include <corn/ui/ui_widget.h>
#include <cornui/xml/easing_function.h>

namespace cornui {
    class DOM;

    class DOMNode {
    public:
        friend class DOM;
        friend class UI;

        struct Animation {
            std::string name;
            std::string startValue;
            std::string endValue;
            std::unique_ptr<EasingFunction> ease;
            float totalTime;
            float currentTime;

            /// @brief Get the current value of the transformation. If invalid then return the end value.
            [[nodiscard]] std::string getCurrentValue() const;
        };

        DOMNode() noexcept;
        ~DOMNode();

        /// @brief Clears all descendants of the node.
        void clear() noexcept;

        /// @brief Removes all child nodes.
        void clearChildren() noexcept;

        /**
         * @brief Copy constructor & Copy assignment operator.
         *
         * Note: Descendants will be copied and attached. The parent_ property will point to null point (target node
         * only).
         */
        DOMNode(const DOMNode& other) noexcept;
        DOMNode& operator=(const DOMNode& other) noexcept;

        /// @return The inner XML as a string.
        [[nodiscard]] std::string getInnerXML() const noexcept;

        /// @return The inner XML as a string.
        [[nodiscard]] std::string getOuterXML() const noexcept;

        /**
         * @brief Set the inner XML of the node.
         * @param content The content to parse as inner XML.
         */
        void setInnerXML(const std::string& content);

        /**
         * @brief Create a new DOM node from the given outer XML.
         * @param dom The DOM object.
         * @param outerXML The outer XML of the node.
         * @param isDefNode Whether the node to construct is the root of a definition node.
         * @param allowTextNode Whether to allow node with tag <text>.
         */
        static DOMNode fromOuterXML(
                DOM* dom, const std::string& outerXML, bool isDefNode = false, bool allowTextNode = false);

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

        /// @brief Sync the node with the UI.
        void sync();

        /**
         * @brief Transform the style smoothly with an animation.
         * @param name The name of the style to animate.
         * @param value The end value to transform into.
         * @param ease The easing function to use.
         * @param duration Duration of the animation.
         * @return Whether the animation is executed. If not, will set the end value directly.
         */
        bool animate(const std::string& name, const std::string& value, std::unique_ptr<EasingFunction> ease, float duration) noexcept;

        // Getters & Setters
        [[nodiscard]] const std::string& getTag() const noexcept;
        [[nodiscard]] const std::string& getName() const noexcept;
        void setName(const std::string& name) noexcept;
        [[nodiscard]] const std::u8string& getLocalText() const noexcept;
        [[nodiscard]] std::u8string getText() const noexcept;
        void setText(const std::u8string& text) noexcept;
        [[nodiscard]] corn::RichText getRichText() const;
        void setRichText(const corn::RichText& richText);
        [[nodiscard]] const std::vector<std::string>& getClassList() const noexcept;
        [[nodiscard]] const std::unordered_map<std::string, std::string>& getStyle() const noexcept;
        void setStyle(const std::string& name, const std::string& value, bool resetAnimation = true) noexcept;
        [[nodiscard]] const std::unordered_map<std::string, std::string>& getComputedStyle() const noexcept;
        [[nodiscard]] corn::Vec4 getComputedGeometry() const noexcept;
        [[nodiscard]] const std::unordered_map<std::string, std::string>& getAttributes() const noexcept;
        void setAttribute(const std::string& name, const std::string& value) noexcept;
        void removeAttribute(const std::string& name) noexcept;
        [[nodiscard]] std::unordered_map<std::string, Animation>& getAnimations() noexcept;

        [[nodiscard]] DOM* getDOM() const noexcept;
        [[nodiscard]] DOMNode* getParent() const noexcept;
        [[nodiscard]] const std::vector<DOMNode*>& getChildren() const noexcept;
        [[nodiscard]] corn::UIWidget::WidgetID getWidgetID() const noexcept;
        [[nodiscard]] corn::UIWidget* getWidget() const noexcept;

    private:
        /// @brief Sync the children of the node with the UI.
        void syncChildren() const;

        /// @brief Computes the style of an element node.
        void computeElementStyle();

        /// @brief Computes the style of a text node.
        void computeTextStyle();

        /// @brief Computes the style of a node.
        void computeStyle();

        /**
         * @brief Computes the style of a node.
         * @param inheritedStyles Styles inherited from parent nodes.
         */
        void computeStyle(const std::unordered_map<std::string, std::string>& inheritedStyles);

        /**
         * @brief If current node is a text node, set the inner XML as text.
         * @param content The content to set.
         */
        void setInnerXMLAsText(const std::string& content) noexcept;

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
         * @param value The value to pass in.
         *
         * This function is automatically triggered by corresponding events.
         */
        void runScriptInAttr(const std::string& attr, float value);

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
        std::unordered_map<std::string, Animation> animations_;
        std::unordered_map<std::string, std::string> attributes_;

        DOM* dom_;
        DOMNode* parent_;
        std::vector<DOMNode*> children_;
        corn::UIWidget::WidgetID widgetID_;
    };
}
