#pragma once

#include <string>
#include <corn/event/event_args.h>
#include <cornui/xml/dom_node.h>

namespace cornui {
    class EventArgsAnimation : public corn::EventArgs {
    public:
        [[nodiscard]] std::string type() const noexcept override { return "cornui::animation::start"; }

        cornui::DOMNode* node;

        explicit EventArgsAnimation(cornui::DOMNode* node) : node(node) {}
    };
}
