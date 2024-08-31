#pragma once

#include <duktape.h>
#include <corn/event/event_scope.h>

namespace cornui {
    /**
     *
     */
    class JSRuntimeImpl {
    public:
        duk_context* ctx_;

        explicit JSRuntimeImpl(DOM& dom);
        ~JSRuntimeImpl();

    private:
        corn::EventScope eventScope_;
    };
}
