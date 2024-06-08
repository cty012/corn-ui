#pragma once

#include <duktape.h>

namespace cornui {
    /**
     *
     */
    class JSRuntimeImpl {
    public:
        duk_context* ctx_;

        explicit JSRuntimeImpl(DOM& dom);
        ~JSRuntimeImpl();
    };
}
