#include "console.h"

namespace cornui {
    void create_console(duk_context* ctx) {
        // Push the global object and "document" object onto the stack
        duk_push_global_object(ctx);
        duk_idx_t consoleIdx = duk_push_object(ctx);

        // Add function "log" to "console"
        duk_push_c_function(ctx, console_log, 1);
        duk_put_prop_string(ctx, consoleIdx, "log");

        // Add function "error" to "console"
        duk_push_c_function(ctx, console_error, 1);
        duk_put_prop_string(ctx, consoleIdx, "error");

        // Add "console" to the global object
        duk_put_global_string(ctx, "console");

        // Pop the global object
        duk_pop(ctx);
    }

    duk_ret_t console_log(duk_context* ctx) {
        printf("[JS runtime log] %s\n", duk_to_string(ctx, 0));
        return 0;
    }

    duk_ret_t console_error(duk_context* ctx) {
        printf("[JS runtime error] %s\n", duk_to_string(ctx, 0));
        return 0;
    }
}
