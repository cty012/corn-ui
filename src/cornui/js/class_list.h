#pragma once

#include <duktape.h>

namespace cornui {
    class DOMNode;

    void create_classList_prototype(duk_context* ctx);
    void push_classList(duk_context* ctx, DOMNode* node);

    duk_ret_t classList_toArray(duk_context* ctx);
    duk_ret_t classList_toString(duk_context* ctx);
    duk_ret_t classList_contains(duk_context* ctx);
    duk_ret_t classList_item(duk_context* ctx);
    duk_ret_t classList_add(duk_context* ctx);
    duk_ret_t classList_remove(duk_context* ctx);
    duk_ret_t classList_toggle(duk_context* ctx);
    duk_ret_t classList_replace(duk_context* ctx);
}
