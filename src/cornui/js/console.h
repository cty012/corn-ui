#pragma once

#include <duktape.h>

namespace cornui {
    void create_console(duk_context* ctx);

    duk_ret_t console_log(duk_context* ctx);
    duk_ret_t console_error(duk_context* ctx);
}
