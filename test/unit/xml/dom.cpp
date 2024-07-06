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
        vnodes[0] = { .vnodeName = "1.0", .tag = "body" };
        vnodes[1] = { .vnodeName = "1.1", .tag = "widget", .name = "body" };
        vnodes[2] = { .vnodeName = "1.2", .tag = "label", .classList = { "text" } };
        vnodes[3] = { .vnodeName = "1.3", .tag = "text", .text = u8"Hello world" };
        vnodes[4] = { .vnodeName = "1.4", .tag = "image", .attributes = {{ "src", "../../image/corn-logo.png" }} };
        clearAllConnections();
        nodeConnect(&vnodes[0], &vnodes[1]);
        nodeConnect(&vnodes[1], &vnodes[2]);
        nodeConnect(&vnodes[2], &vnodes[3]);
        nodeConnect(&vnodes[1], &vnodes[4]);
        nodeCompare(&ui.getDOM()->getRoot(), &vnodes[0]);

        // Simple XML+CCSS+JS file
        ui.init("resources/ui/views/simple_js.xml", scene.getUIManager());
        vnodes[0] = { .vnodeName = "2.0", .tag = "body" };
        vnodes[1] = { .vnodeName = "2.1", .tag = "widget", .name = "body" };
        vnodes[2] = { .vnodeName = "2.2", .tag = "label", .classList = { "text" } };
        vnodes[3] = { .vnodeName = "2.3", .tag = "text", .text = u8"Hello JavaScript" };
        vnodes[4] = { .vnodeName = "2.4", .tag = "image", .attributes = {{ "src", "../../image/corn-logo.png" }} };
        clearAllConnections();
        nodeConnect(&vnodes[0], &vnodes[1]);
        nodeConnect(&vnodes[1], &vnodes[2]);
        nodeConnect(&vnodes[2], &vnodes[3]);
        nodeConnect(&vnodes[1], &vnodes[4]);
        nodeCompare(&ui.getDOM()->getRoot(), &vnodes[0]);

        // Def file
        ui.init("resources/ui/views/load_def.xml", scene.getUIManager());
        vnodes[0] = { .vnodeName = "3.0", .tag = "body" };
        vnodes[1] = { .vnodeName = "3.1", .tag = "widget", .name = "body" };
        vnodes[2] = { .vnodeName = "3.2", .tag = "widget", .classList = { "text" } };
        vnodes[3] = { .vnodeName = "3.3", .tag = "label", };
        vnodes[4] = { .vnodeName = "3.4", .tag = "text", .text = u8"My tag" };
        vnodes[5] = { .vnodeName = "3.5", .tag = "image", .attributes = {{ "src", "../../image/corn-logo.png" }} };
        clearAllConnections();
        nodeConnect(&vnodes[0], &vnodes[1]);
        nodeConnect(&vnodes[1], &vnodes[2]);
        nodeConnect(&vnodes[2], &vnodes[3]);
        nodeConnect(&vnodes[3], &vnodes[4]);
        nodeConnect(&vnodes[1], &vnodes[5]);
        nodeCompare(&ui.getDOM()->getRoot(), &vnodes[0]);
    }
}
