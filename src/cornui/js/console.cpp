#include <sstream>
#include "common.h"
#include "console.h"

namespace cornui {
    void create_console(JSContext* ctx) {
        // Create empty "console" object
        JSValue global = JS_GetGlobalObject(ctx);
        JSValue console = JS_NewObject(ctx);

        // Attach "log" function
        JS_SetPropertyStr(
                ctx, console, "log",
                JS_NewCFunction(ctx, js_console_log, "log", 1));

        // Attach "error" function
        JS_SetPropertyStr(
                ctx, console, "error",
                JS_NewCFunction(ctx, js_console_error, "error", 1));

        // Attach "console" to the global object
        JS_SetPropertyStr(
                ctx, global, "console",
                console);

        // Cleanup
        JS_FreeValue(ctx, global);
    }

    JSValue js_console_log(JSContext* ctx, JSValueConst, int argc, JSValueConst* argv) {
        if (argc == 0) return JS_UNDEFINED;

        // Concatenate the items
        std::stringstream ss;
        bool isFirst = true;
        for (int i = 0; i < argc; i++) {
            // Skip if the item cannot be converted to string
            std::string item;
            if (!getString(ctx, &item, argv[i])) continue;

            // Print the item
            if (!isFirst) ss << " ";
            isFirst = false;
            ss << item;
        }

        printf("[JS runtime log] %s\n", ss.str().c_str());
        return JS_UNDEFINED;
    }

    JSValue js_console_error(JSContext* ctx, JSValueConst, int argc, JSValueConst* argv) {
        if (argc == 0) return JS_UNDEFINED;

        // Concatenate the items
        std::stringstream ss;
        bool isFirst = true;
        for (int i = 0; i < argc; i++) {
            // Skip if the item cannot be converted to string
            std::string item;
            if (!getString(ctx, &item, argv[i])) continue;

            // Print the item
            if (!isFirst) ss << " ";
            isFirst = false;
            ss << item;
        }

        fprintf(stderr, "[JS runtime error] %s\n", ss.str().c_str());
        return JS_UNDEFINED;
    }
}
