#include <corn/core/scene.h>
#include <cornui/css/animation.h>
#include <cornui/xml/dom.h>
#include "event_animation.h"

namespace cornui {
    SAnimation::SAnimation(corn::Scene& scene, cornui::UI* ui) : corn::System(scene), ui_(ui) {
        // Listen to animation start events
        this->eventScope_.addListener(
                this->getScene().getEventManager(),
                "cornui::animation::start",
                [this](const corn::EventArgs& args) {
                    const auto& animationArgs = dynamic_cast<const EventArgsAnimation&>(args);
                    if (&animationArgs.node->getDOM()->getUI() != this->ui_) return;
                    this->animatingNodes_.insert(animationArgs.node);
                });
    }

    void SAnimation::update(float millis) {
        for (auto it = this->animatingNodes_.begin(); it != this->animatingNodes_.end();) {
            DOMNode* node = *it;
            if (node->getAnimations().empty()) {
                it = this->animatingNodes_.erase(it);
                continue;
            }

            std::unordered_map<std::string, DOMNode::Animation>& animations = node->getAnimations();
            std::unordered_set<std::string> toRemove;

            for (auto& [name, animation] : animations) {
                // Update time and style
                animation.currentTime += millis;
                if (animation.currentTime >= animation.totalTime) {
                    animation.currentTime = animation.totalTime;
                    node->setStyle(name, animation.endValue);
                    toRemove.insert(name);
                } else {
                    node->setStyle(name, animation.getCurrentValue());
                }
            }

            // Remove animations that are done
            for (const std::string& name : toRemove) {
                animations.erase(name);
            }

            node->sync();
            ++it;
        }
    }
}
