#pragma once

#include <corn/event/event_args.h>

namespace cornui {
    struct EventArgsJS : public corn::EventArgs {
        /// @return Type of the event. Specified by the first argument in JS.
        [[nodiscard]] std::string type() const noexcept override { return this->type_; }

        std::string type_;
        std::string payload;

        /// @brief Constructor.
        EventArgsJS(std::string type, std::string payload);
    };
}
