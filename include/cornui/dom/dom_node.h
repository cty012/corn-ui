#pragma once

#include <string>
#include <unordered_map>
#include <vector>

namespace cornui {
    struct DOMNode {
        std::u8string tag;
        std::u8string text;
        std::vector<std::u8string> classList;
        std::unordered_map<std::u8string, std::u8string> attributes;
        std::vector<DOMNode*> children;
    };

    /**
     * Clears all descendants of the node.
     * @param node The target node to clear.
     */
    void clearNode(DOMNode* node);

    /**
     * Duplicates the given node and all its descendants.
     * @param node The target node to duplicate.
     * @return The duplicate node.
     */
    DOMNode* dupNode(const DOMNode* node);

    /**
     * @param node The target node.
     * @return The inner XML as a string.
     */
    std::u8string getInnerXML(const DOMNode& node);

    /**
     * @param node The target node.
     * @return The inner XML as a string.
     */
    std::u8string getOuterXML(const DOMNode& node);
}
