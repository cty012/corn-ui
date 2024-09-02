#pragma once

#include <duktape.h>
#include <cornui/js/request.h>

namespace cornui {
    void create_request(duk_context* ctx, const std::string& name, Request* request);

    duk_ret_t request_get(duk_context* ctx);
}
