#pragma once

#include <string>
#include <duktape.h>

namespace cornui {
    class DOM;

    void create_document(duk_context* ctx, DOM* dom);

    duk_ret_t document_root_get(duk_context* ctx);
    duk_ret_t document_getNodeBySelector(duk_context* ctx);
    duk_ret_t document_getNodesBySelector(duk_context* ctx);
}
