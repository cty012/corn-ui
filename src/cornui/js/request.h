#pragma once

#include <quickjs/quickjs.h>

namespace cornui {
    void create_request(JSContext* ctx);

    JSValue js_request_get(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv);
}
