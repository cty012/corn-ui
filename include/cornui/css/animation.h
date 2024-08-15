#pragma once

#include <unordered_set>
#include <corn/ecs/system.h>
#include <corn/event/event_scope.h>
#include <cornui/xml/dom_node.h>

namespace cornui {
    class UI;

    class SAnimation : public corn::System {
    public:
        SAnimation(corn::Scene& scene, cornui::UI* ui);

        void update(float millis) override;

    private:
        cornui::UI* ui_;
        std::unordered_set<cornui::DOMNode*> animatingNodes_;
        corn::EventScope eventScope_;
    };
}
