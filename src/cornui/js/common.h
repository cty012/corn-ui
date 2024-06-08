#pragma once

#include <duktape.h>

namespace cornui {
    void push_prototype(duk_context* ctx, const char* name);

    template<typename T>
    T* getPtr(duk_context* ctx) {
        // Push "this" (the DOMNode) and the DOMNode pointer onto the stack
        duk_push_this(ctx);
        duk_get_prop_string(ctx, -1, DUK_HIDDEN_SYMBOL("__ptr"));

        // Retrieve the pointer
        T* node = static_cast<T*>(duk_to_pointer(ctx, -1));

        // Pop "this" and the pointer
        duk_pop_2(ctx);

        return node;
    }
}
