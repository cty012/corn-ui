#pragma once

#include <string>
#include <duktape.h>
#include <corn/event/event_manager.h>
#include <corn/event/event_scope.h>

namespace cornui {
    void create_event(duk_context* ctx, const std::string& name, corn::EventManager& eventManager, corn::EventScope& eventScope);

    duk_ret_t event_emit(duk_context* ctx);
    duk_ret_t event_listen(duk_context* ctx);
}
