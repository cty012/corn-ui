#pragma once

#include <quickjs/quickjs.h>

namespace cornui {
    class DOMNode;

    void create_ClassList(JSContext* ctx);
    JSValue js_classList(JSContext* ctx, DOMNode* node);

    JSValue js_classList_toArray(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv);
    JSValue js_classList_toString(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv);
    JSValue js_classList_contains(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv);
    JSValue js_classList_item(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv);
    JSValue js_classList_add(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv);
    JSValue js_classList_remove(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv);
    JSValue js_classList_toggle(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv);
    JSValue js_classList_replace(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv);
}
