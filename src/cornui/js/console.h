#pragma once

#include <quickjs/quickjs.h>

namespace cornui {
    void create_console(JSContext* ctx);

    JSValue js_console_log(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv);
    JSValue js_console_error(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv);
}
