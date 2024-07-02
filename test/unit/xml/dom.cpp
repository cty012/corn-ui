#include <gtest/gtest.h>
#include <cornui/ui.h>
#include "dummy_scene.h"
#include "virtual_node.h"

namespace cornui::test::dom {
    TEST(DOM, init) {
        cornui::UI ui;
        DummyScene scene;
        std::vector<VirtualNode> vnodes(10);
        std::function<void(void)> clearAllConnections = [&vnodes]() {
            for (VirtualNode& vnode : vnodes) {
                vnode.parent = nullptr;
                vnode.children.clear();
            }
        };

        // Simple XML+CCSS file
        ui.init("resources/ui/views/simple.xml", scene.getUIManager());
        vnodes[0] = { .vnodeName = "0", .tag = "body" };
        vnodes[1] = { .vnodeName = "1", .tag = "widget", .name = "body" };
        vnodes[2] = { .vnodeName = "2", .tag = "label", .text = u8"Hello world", .classList = { "text" } };
        vnodes[3] = { .vnodeName = "3", .tag = "image", .attributes = {{ "src", "../../image/corn-logo.png" }} };
        clearAllConnections();
        nodeConnect(&vnodes[0], &vnodes[1]);
        nodeConnect(&vnodes[1], &vnodes[2]);
        nodeConnect(&vnodes[1], &vnodes[3]);
        nodeCompare(&ui.getDOM()->getRoot(), &vnodes[0]);

        // Simple XML+CCSS+JS file
        ui.init("resources/ui/views/simple_js.xml", scene.getUIManager());
        vnodes[0] = { .vnodeName = "0", .tag = "body" };
        vnodes[1] = { .vnodeName = "1", .tag = "widget", .name = "body" };
        vnodes[2] = { .vnodeName = "2", .tag = "label", .text = u8"Hello JavaScript", .classList = { "text" } };
        vnodes[3] = { .vnodeName = "3", .tag = "image", .attributes = {{ "src", "../../image/corn-logo.png" }} };
        clearAllConnections();
        nodeConnect(&vnodes[0], &vnodes[1]);
        nodeConnect(&vnodes[1], &vnodes[2]);
        nodeConnect(&vnodes[1], &vnodes[3]);
        nodeCompare(&ui.getDOM()->getRoot(), &vnodes[0]);

        // Def file
        ui.init("resources/ui/views/load_def.xml", scene.getUIManager());
        vnodes[0] = { .vnodeName = "0", .tag = "body" };
        vnodes[1] = { .vnodeName = "1", .tag = "widget", .name = "body" };
        vnodes[2] = { .vnodeName = "2", .tag = "widget", .classList = { "text" } };
        vnodes[3] = { .vnodeName = "3", .tag = "label", .text = u8"My tag" };
        vnodes[4] = { .vnodeName = "4", .tag = "image", .attributes = {{ "src", "../../image/corn-logo.png" }} };
        clearAllConnections();
        nodeConnect(&vnodes[0], &vnodes[1]);
        nodeConnect(&vnodes[1], &vnodes[2]);
        nodeConnect(&vnodes[2], &vnodes[3]);
        nodeConnect(&vnodes[1], &vnodes[4]);
        nodeCompare(&ui.getDOM()->getRoot(), &vnodes[0]);
    }
}
