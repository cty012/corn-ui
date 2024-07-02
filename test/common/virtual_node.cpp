#include <format>
#include <gtest/gtest.h>
#include "utility.h"
#include "virtual_node.h"

namespace cornui::test {
    void nodeConnect(VirtualNode* parent, VirtualNode* child) {
        if (std::find(parent->children.begin(), parent->children.end(), child) == parent->children.end()) {
            parent->children.push_back(child);
        }
        child->parent = parent;
    }

    bool nodeCompare(const DOMNode* domNode, const VirtualNode* virtualNode) {
        if (!domNode && !virtualNode) {
            return true;
        } else if (!domNode || !virtualNode) {
            return false;
        }

        std::string msg = std::format("Failed on virtual node \"{}\"", virtualNode->vnodeName);

        // Compare current node
        EXPECT_EQ_MSG_RETURN(domNode->getTag(), virtualNode->tag, msg, false);
        EXPECT_EQ_MSG_RETURN(domNode->getName(), virtualNode->name, msg, false);
        std::string domText = (const char*)domNode->getText().c_str();
        std::string virtualText = (const char*)virtualNode->text.c_str();
        EXPECT_EQ_MSG_RETURN(domText, virtualText, msg, false);
        EXPECT_EQ_MSG_RETURN(
                VectorsEqual(domNode->getClassList(), virtualNode->classList), true,
                msg, false);
        if (!UnorderedMapsEqual(domNode->getAttributes(), virtualNode->attributes)) {
            printf("%s [attributes]\n", msg.c_str());
            return false;
        }
        if (!UnorderedMapsEqual(domNode->getStyle(), virtualNode->style)) {
            printf("%s [style]\n", msg.c_str());
            return false;
        }

        // Compare children
        const std::vector<DOMNode*>& domChildren = domNode->getChildren();
        const std::vector<VirtualNode*>& virtualChildren = virtualNode->children;
        EXPECT_EQ_MSG_RETURN(domChildren.size(), virtualChildren.size(), msg, false);
        for (size_t i = 0; i < domChildren.size(); i++) {
            if (!nodeCompare(domChildren.at(i), virtualChildren.at(i))) {
                return false;
            }
        }

        return true;
    }
}
