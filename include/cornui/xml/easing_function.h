#pragma once

#include <string>

namespace cornui {
    /**
     * @class EaseFunction
     * @brief Base class for all easing functions.
     */
    class EasingFunction {
    public:
        virtual ~EasingFunction() = 0;

        /// @brief Calculate the easing value at the given time.
        virtual float operator()(float time) const noexcept = 0;
    };

    /**
     * @class EasingLinear
     * @brief Linear easing function.
     */
    class EasingLinear : public EasingFunction {
    public:
        ~EasingLinear() override;

        [[nodiscard]] float operator()(float time) const noexcept override;
    };

    /**
     * @class EasingStepStart
     * @brief Jumps to the end value at the start of the animation.
     */
    class EasingStepStart : public EasingFunction {
    public:
        ~EasingStepStart() override;

        [[nodiscard]] float operator()(float time) const noexcept override;
    };

    /**
     * @class EasingStepEnd
     * @brief Jumps to the end value by the end of the animation.
     */
    class EasingStepEnd : public EasingFunction {
    public:
        ~EasingStepEnd() override;

        [[nodiscard]] float operator()(float time) const noexcept override;
    };

    /**
     * @class EasingBezier
     * @brief Bezier easing function.
     */
    class EasingBezier : public EasingFunction {
    public:
        EasingBezier(float x1, float y1, float x2, float y2);

        ~EasingBezier() override;

        [[nodiscard]] float operator()(float time) const noexcept override;

    protected:
        static constexpr float err_ = 0.0005f;
        float x1_, y1_, x2_, y2_;
    };

    /**
     * @class EasingEase
     * @brief Default animation.
     */
    class EasingEase : public EasingBezier {
    public:
        EasingEase();
    };

    /**
     * @class EasingEaseIn
     * @brief Start smoothly and speed up by the end.
     */
    class EasingEaseIn : public EasingBezier {
    public:
        EasingEaseIn();
    };

    /**
     * @class EasingEaseOut
     * @brief Abruptly start and slow down smoothly by the end.
     */
    class EasingEaseOut : public EasingBezier {
    public:
        EasingEaseOut();
    };

    /**
     * @class EasingEaseInOut
     * @brief Start and end smoothly, like an "S" curve.
     */
    class EasingEaseInOut : public EasingBezier {
    public:
        EasingEaseInOut();
    };
}
