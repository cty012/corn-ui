#include <cornui/xml/easing_function.h>

namespace cornui {
    EasingFunction::~EasingFunction() = default;

    EasingLinear::~EasingLinear() = default;

    float EasingLinear::operator()(float time) const noexcept {
        return time;
    }

    EasingStepStart::~EasingStepStart() = default;

    float EasingStepStart::operator()(float time) const noexcept {
        return time == 0.0f ? 1.0f : 0.0f;
    }

    EasingStepEnd::~EasingStepEnd() = default;

    float EasingStepEnd::operator()(float time) const noexcept {
        return time == 1.0f ? 1.0f : 0.0f;
    }

    EasingBezier::EasingBezier(float x1, float y1, float x2, float y2) : x1_(x1), y1_(y1), x2_(x2), y2_(y2) {}

    float bezier(float t, float p1, float p2) {
        float t2 = t * t;
        float t3 = t2 * t;
        float mt = 1.0f - t;
        float mt2 = mt * mt;
        return 3.0f * mt2 * t * p1 + 3.0f * mt * t2 * p2 + t3;
    }

    float EasingBezier::operator()(float time) const noexcept {
        // Solve for the parametric value of t
        float tmin = 0.0f;
        float tmax = 1.0f;
        while (tmax - tmin > 2 * EasingBezier::err_) {
            float t = (tmin + tmax) / 2.0f;
            float x = bezier(t, x1_, x2_);
            if (x < time) {
                tmin = t;
            } else {
                tmax = t;
            }
        }

        // Return the y value
        return bezier((tmin + tmax) / 2.0f, y1_, y2_);
    }

    EasingBezier::~EasingBezier() = default;

    EasingEase::EasingEase() : EasingBezier(0.25f, 0.1f, 0.25f, 1.0f) {}

    EasingEaseIn::EasingEaseIn() : EasingBezier(0.42f, 0.0f, 1.0f, 1.0f) {}

    EasingEaseOut::EasingEaseOut() : EasingBezier(0.0f, 0.0f, 0.58f, 1.0f) {}

    EasingEaseInOut::EasingEaseInOut() : EasingBezier(0.42f, 0.0f, 0.58f, 1.0f) {}
}
