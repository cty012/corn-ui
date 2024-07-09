#include <cornui/js/js_event_args.h>
#include "common.h"
#include "event.h"

namespace cornui {
    void create_event(duk_context* ctx, const std::string& name, corn::EventManager& eventManager, corn::EventScope& eventScope) {
        // Add "listeners" object to the global stash
        duk_push_global_stash(ctx);
        duk_push_object(ctx);
        duk_put_prop_string(ctx, -2, DUK_HIDDEN_SYMBOL("__listeners"));
        duk_pop(ctx);

        // Push the event object onto the stack
        duk_push_global_object(ctx);
        duk_idx_t eventIdx = duk_push_object(ctx);

        // Add the event manager pointer as hidden property
        duk_push_pointer(ctx, &eventManager);
        duk_put_prop_string(ctx, eventIdx, DUK_HIDDEN_SYMBOL("__manager"));

        // Add the event scope pointer as hidden property
        duk_push_pointer(ctx, &eventScope);
        duk_put_prop_string(ctx, eventIdx, DUK_HIDDEN_SYMBOL("__scope"));

        // Add "emit" function to the event object
        duk_push_c_function(ctx, event_emit, DUK_VARARGS);
        duk_put_prop_string(ctx, eventIdx, "emit");

        // Add "listen" function to the event object
        duk_push_c_function(ctx, event_listen, 2);
        duk_put_prop_string(ctx, eventIdx, "listen");

        // Add event object to the global object
        duk_put_global_string(ctx, name.c_str());

        // Pop the global object
        duk_pop(ctx);
    }

    duk_ret_t event_emit(duk_context* ctx) {
        duk_idx_t nargs = duk_get_top(ctx);
        auto* eventManager = getPtr<corn::EventManager>(ctx, "__manager");

        if ((nargs == 1 || nargs == 2) && eventManager) {
            const char* type = duk_get_string(ctx, 0);
            const char* payload = (nargs == 1) ? "" : duk_get_string(ctx, 1);

            if (type && payload) {
                eventManager->emit(EventArgsJS("js::" + std::string(type), payload));
            }
        }

        return 0;
    }

    duk_ret_t event_listen(duk_context* ctx) {
        static size_t key = 0;

        auto* eventManager = getPtr<corn::EventManager>(ctx, "__manager");
        auto* eventScope = getPtr<corn::EventScope>(ctx, "__scope");

        if (eventManager && eventScope) {
            const char* type = duk_get_string(ctx, 0);

            if (type && duk_is_function(ctx, 1)) {
                // Retrieve the listener
                duk_push_global_stash(ctx);
                duk_get_prop_string(ctx, -1, DUK_HIDDEN_SYMBOL("__listeners"));

                // Store the key-function pair in the listeners object
                duk_dup(ctx, 1);  // Duplicate the function
                std::string keyStr = std::to_string(key);
                duk_put_prop_string(ctx, -2, keyStr.c_str());

                // Add the listener to the event manager
                eventScope->addListener(
                        *eventManager,
                        "js::" + std::string(type),
                        [ctx, keyStr](const corn::EventArgs& args) {
                            const auto& args_ = dynamic_cast<const EventArgsJS&>(args);

                            // Retrieve the listener
                            duk_push_global_stash(ctx);
                            duk_get_prop_string(ctx, -1, DUK_HIDDEN_SYMBOL("__listeners"));
                            duk_get_prop_string(ctx, -1, keyStr.c_str());

                            // Call the listener
                            duk_push_string(ctx, args_.payload.c_str());
                            duk_call(ctx, 1);

                            // Clean up (return value, listeners object, global stash)
                            duk_pop_3(ctx);
                        });

                // Increment the key & clean up
                key++;
                duk_pop_2(ctx);
            }
        }

        return 0;
    }
}
