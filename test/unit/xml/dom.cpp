#include <gtest/gtest.h>
#include <cornui/js/js_event_args.h>
#include <cornui/ui.h>
#include <cornui/util/css_parser.h>
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
        vnodes[2] = { .vnodeName = "1.2", .tag = "label", .text = u8"Hello world", .classList = { "text" } };
        vnodes[3] = { .vnodeName = "1.3", .tag = "image", .attributes = {{ "src", "../../image/corn-logo.png" }} };
        clearAllConnections();
        nodeConnect(&vnodes[0], &vnodes[1]);
        nodeConnect(&vnodes[1], &vnodes[2]);
        nodeConnect(&vnodes[1], &vnodes[3]);
        nodeCompare(&ui.getDOM()->getRoot(), &vnodes[0]);

        // Simple XML+CCSS+JS file
        ui.init("resources/ui/views/simple_js.xml", scene.getUIManager());
        vnodes[0] = { .vnodeName = "2.0", .tag = "body" };
        vnodes[1] = { .vnodeName = "2.1", .tag = "widget", .name = "body" };
        vnodes[2] = { .vnodeName = "2.2", .tag = "label", .text = u8"Hello JavaScript", .classList = { "text" } };
        vnodes[3] = { .vnodeName = "2.3", .tag = "image", .attributes = {{ "src", "../../image/corn-logo.png" }} };
        clearAllConnections();
        nodeConnect(&vnodes[0], &vnodes[1]);
        nodeConnect(&vnodes[1], &vnodes[2]);
        nodeConnect(&vnodes[1], &vnodes[3]);
        nodeCompare(&ui.getDOM()->getRoot(), &vnodes[0]);

        // Def file
        ui.init("resources/ui/views/load_def.xml", scene.getUIManager());
        vnodes[0] = { .vnodeName = "3.0", .tag = "body" };
        vnodes[1] = { .vnodeName = "3.1", .tag = "widget", .name = "body" };
        vnodes[2] = { .vnodeName = "3.2", .tag = "widget", .classList = { "text" } };
        vnodes[3] = { .vnodeName = "3.3", .tag = "label", .text = u8"My tag" };
        vnodes[4] = { .vnodeName = "3.4", .tag = "image", .attributes = {{ "src", "../../image/corn-logo.png" }} };
        clearAllConnections();
        nodeConnect(&vnodes[0], &vnodes[1]);
        nodeConnect(&vnodes[1], &vnodes[2]);
        nodeConnect(&vnodes[2], &vnodes[3]);
        nodeConnect(&vnodes[1], &vnodes[4]);
        nodeCompare(&ui.getDOM()->getRoot(), &vnodes[0]);
    }

    TEST(DOM, getter_setter) {
        cornui::UI ui;
        DummyScene scene;
        std::vector<VirtualNode> vnodes(10);
        std::function<void(void)> clearAllConnections = [&vnodes]() {
            for (VirtualNode& vnode: vnodes) {
                vnode.parent = nullptr;
                vnode.children.clear();
            }
        };

        // Text
        ui.init("resources/ui/views/simple.xml", scene.getUIManager());
        ui.getDOM()->getNodeBySelector(".text")->setInnerXML(R"(<text>A custom</text> <text>text</text>)");
        vnodes[0] = { .vnodeName = "1.0", .tag = "body" };
        vnodes[1] = { .vnodeName = "1.1", .tag = "widget", .name = "body" };
        vnodes[2] = { .vnodeName = "1.2", .tag = "label", .classList = { "text" } };
        vnodes[3] = { .vnodeName = "1.3", .tag = "text", .text = u8"A custom" };
        vnodes[4] = { .vnodeName = "1.4", .tag = "text", .text = u8"text" };
        vnodes[5] = { .vnodeName = "1.5", .tag = "image", .attributes = {{ "src", "../../image/corn-logo.png" }} };
        clearAllConnections();
        nodeConnect(&vnodes[0], &vnodes[1]);
        nodeConnect(&vnodes[1], &vnodes[2]);
        nodeConnect(&vnodes[2], &vnodes[3]);
        nodeConnect(&vnodes[2], &vnodes[4]);
        nodeConnect(&vnodes[1], &vnodes[5]);
        nodeCompare(&ui.getDOM()->getRoot(), &vnodes[0]);

        bool isMatch = match(parseSelectorFromString(".text text"), *ui.getDOM()->getNodeBySelector(".text")->getChildren()[0]);
        EXPECT_TRUE(isMatch);

        // innerXML
        ui.init("resources/ui/views/empty.xml", scene.getUIManager());
        ui.getDOM()->getNodeBySelector("#body")->setInnerXML(
                R"(<label class="text">Hello world</label><image src="../../image/corn-logo.png"/>)");
        vnodes[0] = { .vnodeName = "2.0", .tag = "body" };
        vnodes[1] = { .vnodeName = "2.1", .tag = "widget", .name = "body" };
        vnodes[2] = { .vnodeName = "2.2", .tag = "label", .text = u8"Hello world", .classList = { "text" } };
        vnodes[3] = { .vnodeName = "2.3", .tag = "image", .attributes = {{ "src", "../../image/corn-logo.png" }} };
        clearAllConnections();
        nodeConnect(&vnodes[0], &vnodes[1]);
        nodeConnect(&vnodes[1], &vnodes[2]);
        nodeConnect(&vnodes[1], &vnodes[3]);
        nodeCompare(&ui.getDOM()->getRoot(), &vnodes[0]);
    }

    TEST(DOM, event) {
        cornui::UI ui;
        DummyScene scene;
        corn::EventScope eventScope;
        std::vector<VirtualNode> vnodes(10);
        std::function<void(void)> clearAllConnections = [&vnodes]() {
            for (VirtualNode& vnode : vnodes) {
                vnode.parent = nullptr;
                vnode.children.clear();
            }
        };

        // Event emitting
        std::string localPayload, globalPayload;
        eventScope.addListener(
                scene.getEventManager(),
                "js::emit::local",
                [&localPayload](const corn::EventArgs& args) {
                    const auto& args_ = dynamic_cast<const EventArgsJS&>(args);
                    localPayload = args_.payload;
                });
        eventScope.addListener(
                "js::emit::global",
                [&globalPayload](const corn::EventArgs& args) {
                    const auto& args_ = dynamic_cast<const EventArgsJS&>(args);
                    globalPayload = args_.payload;
                });
        ui.init("resources/ui/views/events.xml", scene.getUIManager());
        EXPECT_EQ(localPayload, "Local event emitted by JavaScript");
        EXPECT_EQ(globalPayload, "Global event emitted by JavaScript");

        // Event listeners
        ui.init("resources/ui/views/events.xml", scene.getUIManager());
        scene.getEventManager().emit(EventArgsJS("js::listen::local", "Local event listened by JavaScript"));
        corn::EventManager::instance().emit(EventArgsJS("js::listen::global", "Global event listened by JavaScript"));
        vnodes[0] = { .vnodeName = "1.0", .tag = "body" };
        vnodes[1] = { .vnodeName = "1.1", .tag = "label", .name = { "local-event" }, .text = u8"Local event listened by JavaScript" };
        vnodes[2] = { .vnodeName = "1.3", .tag = "label", .name = { "global-event" }, .text = u8"Global event listened by JavaScript" };
        clearAllConnections();
        nodeConnect(&vnodes[0], &vnodes[1]);
        nodeConnect(&vnodes[0], &vnodes[2]);
        nodeCompare(&ui.getDOM()->getRoot(), &vnodes[0]);
    }
}
