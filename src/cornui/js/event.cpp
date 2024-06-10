#include <cornui/js/js_event_args.h>
#include "common.h"
#include "event.h"

namespace cornui {
    void create_event(duk_context* ctx, const std::string& name, corn::EventManager& eventManager) {
        // Push the global object and event object onto the stack
        duk_push_global_object(ctx);
        duk_idx_t eventIdx = duk_push_object(ctx);

        // Add the DOM pointer as hidden property
        duk_push_pointer(ctx, &eventManager);
        duk_put_prop_string(ctx, eventIdx, DUK_HIDDEN_SYMBOL("__ptr"));

        // Attach "emit" function to the event object
        duk_push_c_function(ctx, event_emit, DUK_VARARGS);
        duk_put_prop_string(ctx, eventIdx, "emit");

        // Add event object to the global object
        duk_put_global_string(ctx, name.c_str());

        // Pop the global object
        duk_pop(ctx);
    }

    duk_ret_t event_emit(duk_context* ctx) {
        duk_idx_t nargs = duk_get_top(ctx);
        auto* eventManager = getPtr<corn::EventManager>(ctx);

        if ((nargs == 1 || nargs == 2) && eventManager) {
            const char* type = duk_get_string(ctx, 0);
            const char* payload = (nargs == 1) ? "" : duk_get_string(ctx, 1);

            if (type && payload) {
                eventManager->emit(EventArgsJS("js::" + std::string(type), payload));
            }
        }

        return 0;
    }
}
