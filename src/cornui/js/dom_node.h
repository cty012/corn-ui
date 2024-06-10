#pragma once

#include <duktape.h>

namespace cornui {
    class DOMNode;

    void create_domNode_prototype(duk_context* ctx);
    void push_domNode(duk_context* ctx, DOMNode* node);
    void push_domNodeArray(duk_context* ctx, std::vector<DOMNode*> nodes);

    duk_ret_t domNode_innerXML_get(duk_context* ctx);
    duk_ret_t domNode_outerXML_get(duk_context* ctx);

    duk_ret_t domNode_tag_get(duk_context* ctx);
    duk_ret_t domNode_name_get(duk_context* ctx);
    duk_ret_t domNode_name_set(duk_context* ctx);
    duk_ret_t domNode_text_get(duk_context* ctx);
    duk_ret_t domNode_text_set(duk_context* ctx);
    duk_ret_t domNode_classList_get(duk_context* ctx);
    duk_ret_t domNode_style_get(duk_context* ctx);
    duk_ret_t domNode_getComputedStyle(duk_context* ctx);
    duk_ret_t domNode_setStyle(duk_context* ctx);
    duk_ret_t domNode_attributes_get(duk_context* ctx);
    duk_ret_t domNode_hasAttribute(duk_context* ctx);
    duk_ret_t domNode_getAttribute(duk_context* ctx);
    duk_ret_t domNode_setAttribute(duk_context* ctx);
    duk_ret_t domNode_removeAttribute(duk_context* ctx);
}
