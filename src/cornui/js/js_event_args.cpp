#include <cornui/js/js_event_args.h>

#include <utility>

namespace cornui {
    EventArgsJS::EventArgsJS(std::string type, std::string payload)
            : type_(std::move(type)), payload(std::move(payload)) {}
}
