#pragma once

#include <string>
#include <duktape.h>
#include <corn/event/event_manager.h>

namespace cornui {
    void create_event(duk_context* ctx, const std::string& name, corn::EventManager& eventManager);

    duk_ret_t event_emit(duk_context* ctx);
}
