#pragma once

#include <string>
#include <unordered_map>
#include <vector>

namespace cornui {
    struct DOMNode {
        std::string tag;
        std::string name;
        std::u8string text;
        std::vector<std::string> classList;
        std::unordered_map<std::string, std::string> attributes;
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
    std::string getInnerXML(const DOMNode& node);

    /**
     * @param node The target node.
     * @return The inner XML as a string.
     */
    std::string getOuterXML(const DOMNode& node);
}
