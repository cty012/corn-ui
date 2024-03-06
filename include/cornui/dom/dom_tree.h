#pragma once

#include <string>
#include <cornui/dom/dom_node.h>

namespace cornui {
    class DOMTree {
    public:
        /**
         * Constructor. The DOM tree MUST be loaded from a XML file.
         * @param file The path to the XML file.
         * @throw std::invalid_argument If file cannot be loaded.
         */
        explicit DOMTree(std::string file);

        /**
         * Destructor.
         */
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
}
