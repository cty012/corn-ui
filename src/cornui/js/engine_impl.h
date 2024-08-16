#pragma once

#include <quickjs/quickjs.h>

namespace cornui {
    /**
     *
     */
    class JSEngineImpl {
    public:
        JSRuntime* runtime_;
        JSContext* ctx_;

        explicit JSEngineImpl(DOM& dom);
        ~JSEngineImpl();
    };
}
