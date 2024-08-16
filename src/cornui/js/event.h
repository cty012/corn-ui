#pragma once

#include <string>
#include <quickjs/quickjs.h>
#include <corn/event/event_manager.h>

namespace cornui {
    void create_EventManager(JSContext* ctx);
    JSValue js_event(JSContext* ctx, corn::EventManager& eventManager);

    JSValue js_event_emit(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv);
}
