#pragma once

#include <filesystem>
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
        explicit DOM(std::filesystem::path file);

        /**
         * @brief Bind the data in the DOM tree with the UI manager. Changes in the DOM will sync in the UI.
         * @param uiManager The UI manager for generating the UI.
         * @param dom The DOM tree containing the layout.
         *
         * DO NOT alter the UI manager after binding!!
         */
        void bind(corn::UIManager& uiManager);

        [[nodiscard]] const std::filesystem::path& getFile() const noexcept;
        [[nodiscard]] DOMNode& getRoot() noexcept;
        [[nodiscard]] const DOMNode& getRoot() const noexcept;

    private:
        std::filesystem::path file_;
        DOMNode root_;
    };
}
