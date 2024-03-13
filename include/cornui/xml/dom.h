#pragma once

#include <string>
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
        explicit DOM(std::string file);

        /// @brief Destructor.
        ~DOM();

        DOM(const DOM& other);
        DOM& operator=(const DOM& other);
        DOM(DOM&& other) noexcept;
        DOM& operator=(DOM&& other) noexcept;

        [[nodiscard]] DOMNode& getRoot() noexcept;
        [[nodiscard]] const DOMNode& getRoot() const noexcept;

    private:
        std::string file_;
        DOMNode root_;
    };

    /**
     * @brief Parse the information in the DOM tree and generates the UI.
     * @param uiManager The UI manager for generating the UI.
     * @param dom The DOM tree containing the layout.
     */
    void loadUIFromDOM(corn::UIManager& uiManager, const DOM& dom);
}
