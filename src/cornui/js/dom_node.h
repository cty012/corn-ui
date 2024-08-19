#pragma once

#include <quickjs/quickjs.h>

namespace cornui {
    class DOMNode;

    void create_DOMNode(JSContext* ctx);
    JSValue js_domNode(JSContext* ctx, DOMNode* node);
    JSValue js_domNodeArray(JSContext* ctx, std::vector<DOMNode*> nodes);

    JSValue js_domNode_innerXML_get(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv);
    JSValue js_domNode_innerXML_set(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv);
    JSValue js_domNode_outerXML_get(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv);
    JSValue js_domNode_tag_get(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv);
    JSValue js_domNode_name_get(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv);
    JSValue js_domNode_name_set(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv);
    JSValue js_domNode_text_get(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv);
    JSValue js_domNode_text_set(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv);
    JSValue js_domNode_classList_get(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv);
    JSValue js_domNode_style_get(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv);
    JSValue js_domNode_computedStyle_get(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv);
    JSValue js_domNode_computedGeometry_get(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv);
    JSValue js_domNode_naturalSize_get(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv);
    JSValue js_domNode_setStyle(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv);
    JSValue js_domNode_animate(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv);
    JSValue js_domNode_attributes_get(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv);
    JSValue js_domNode_hasAttribute(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv);
    JSValue js_domNode_getAttribute(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv);
    JSValue js_domNode_setAttribute(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv);
    JSValue js_domNode_removeAttribute(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv);
    JSValue js_domNode_parent_get(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv);
    JSValue js_domNode_children_get(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv);
    JSValue js_domNode_getNodeBySelector(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv);
    JSValue js_domNode_getNodesBySelector(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv);
    JSValue js_domNode_focus(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv);
}
