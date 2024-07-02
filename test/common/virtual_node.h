#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <cornui/xml/dom_node.h>

namespace cornui::test {
    struct VirtualNode {
        std::string vnodeName;
        std::string tag;
        std::string name;
        std::u8string text;
        std::vector<std::string> classList;
        std::unordered_map<std::string, std::string> attributes;
        std::unordered_map<std::string, std::string> style;
        VirtualNode* parent = nullptr;
        std::vector<VirtualNode*> children;
    };

    void nodeConnect(VirtualNode* parent, VirtualNode* child);
    bool nodeCompare(const DOMNode* domNode, const VirtualNode* virtualNode);
}
