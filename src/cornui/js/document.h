#pragma once

#include <string>
#include <quickjs/quickjs.h>

namespace cornui {
    void create_document(JSContext* ctx);

    JSValue js_document_root_get(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv);
    JSValue js_document_getNodeBySelector(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv);
    JSValue js_document_getNodesBySelector(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv);
    JSValue js_document_focusedNode_get(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv);
}
