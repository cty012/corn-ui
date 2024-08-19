#pragma once

#include <quickjs/quickjs.h>
#include <cornui/js/request.h>

namespace cornui {
    void create_Request(JSContext* ctx);
    JSValue js_request(JSContext* ctx, Request* request);

    JSValue js_request_get(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv);
}
