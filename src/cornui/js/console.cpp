#include <sstream>
#include "console.h"

namespace cornui {
    void create_console(duk_context* ctx) {
        // Push the global object and "document" object onto the stack
        duk_push_global_object(ctx);
        duk_idx_t consoleIdx = duk_push_object(ctx);

        // Add function "log" to "console"
        duk_push_c_function(ctx, console_log, DUK_VARARGS);
        duk_put_prop_string(ctx, consoleIdx, "log");

        // Add function "error" to "console"
        duk_push_c_function(ctx, console_error, DUK_VARARGS);
        duk_put_prop_string(ctx, consoleIdx, "error");

        // Add "console" to the global object
        duk_put_global_string(ctx, "console");

        // Pop the global object
        duk_pop(ctx);
    }

    duk_ret_t console_log(duk_context* ctx) {
        int nargs = duk_get_top(ctx);
        if (nargs == 0) return 0;

        // Concatenate the items
        std::stringstream ss;
        for (int i = 0; i < nargs; i++) {
            if (i) ss << " ";
            ss << duk_to_string(ctx, i);
        }

        printf("[JS runtime log] %s\n", ss.str().c_str());
        return 0;
    }

    duk_ret_t console_error(duk_context* ctx) {
        int nargs = duk_get_top(ctx);
        if (nargs == 0) return 0;

        // Concatenate the items
        std::stringstream ss;
        for (int i = 0; i < nargs; i++) {
            if (i) ss << " ";
            ss << duk_to_string(ctx, i);
        }

        printf("[JS runtime error] %s\n", ss.str().c_str());
        return 0;
    }
}
