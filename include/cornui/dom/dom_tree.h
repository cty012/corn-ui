#pragma once

#include <string>
#include <cornui/dom/dom_node.h>

namespace corn {
    class UIManager;
}

namespace cornui {
    /**
     * @class DOMTree
     * @brief A tree structure that represent the DOM of the front end. Note this class is only intended to store data.
     * It is not linked to the actual UI on the screen.
     */
    class DOMTree {
    public:
        /**
         * @brief Constructor. The DOM tree MUST be loaded from a XML file.
         * @param file The path to the XML file.
         * @throw std::invalid_argument If file cannot be opened, or the contents of the file cannot be parsed.
         */
        explicit DOMTree(std::string file);

        /// @brief Destructor.
        ~DOMTree();

        DOMTree(const DOMTree& other);
        DOMTree& operator=(const DOMTree& other);
        DOMTree(DOMTree&& other) noexcept;
        DOMTree& operator=(DOMTree&& other) noexcept;

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
    void loadUIFromDOM(corn::UIManager& uiManager, const DOMTree& dom);
}
