#pragma once

#include <string>
#include <unordered_map>
#include <quickjs/quickjs.h>
#include <corn/event/event_scope.h>

namespace cornui {
    class DOM;

    struct ContextData {
        DOM* dom;
        std::unordered_map<std::string, JSClassID> classIDs;

        explicit ContextData(DOM* dom);
    };
}
