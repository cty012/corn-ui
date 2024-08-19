#pragma once

#include <quickjs/quickjs.h>
#include <cornui/js/request.h>

namespace cornui {
    /**
     *
     */
    class JSEngineImpl {
    public:
        JSRuntime* runtime_;
        JSContext* ctx_;

        JSEngineImpl(DOM& dom, Request& request);
        ~JSEngineImpl();
    };
}
