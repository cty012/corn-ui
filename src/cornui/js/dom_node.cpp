#include <cornui/xml/dom_node.h>
#include "class_list.h"
#include "common.h"
#include "dom_node.h"

namespace cornui {
    void create_domNode_prototype(duk_context* ctx) {
        // Push the global stash and the prototype object onto the stack
        duk_push_global_stash(ctx);
        duk_idx_t nodeIdx = duk_push_object(ctx);

        // Attach "innerXML" property to the prototype
        duk_push_string(ctx, "innerXML");
        duk_push_c_function(ctx, domNode_innerXML_get, 0);
        duk_def_prop(ctx, nodeIdx, DUK_DEFPROP_HAVE_GETTER | DUK_DEFPROP_ENUMERABLE);

        // Attach "outerXML" property to the prototype
        duk_push_string(ctx, "outerXML");
        duk_push_c_function(ctx, domNode_outerXML_get, 0);
        duk_def_prop(ctx, nodeIdx, DUK_DEFPROP_HAVE_GETTER | DUK_DEFPROP_ENUMERABLE);

        // Attach "toString" function to the prototype
        duk_push_c_function(ctx, domNode_outerXML_get, 0);
        duk_put_prop_string(ctx, nodeIdx, "toString");

        // Attach "tag" property to the prototype
        duk_push_string(ctx, "tag");
        duk_push_c_function(ctx, domNode_tag_get, 0);
        duk_def_prop(ctx, nodeIdx, DUK_DEFPROP_HAVE_GETTER | DUK_DEFPROP_ENUMERABLE);

        // Attach "name" property to the prototype
        duk_push_string(ctx, "name");
        duk_push_c_function(ctx, domNode_name_get, 0);
        duk_push_c_function(ctx, domNode_name_set, 1);
        duk_def_prop(ctx, nodeIdx, DUK_DEFPROP_HAVE_GETTER | DUK_DEFPROP_HAVE_SETTER | DUK_DEFPROP_ENUMERABLE);

        // Attach "text" property to the prototype
        duk_push_string(ctx, "text");
        duk_push_c_function(ctx, domNode_text_get, 0);
        duk_push_c_function(ctx, domNode_text_set, 1);
        duk_def_prop(ctx, nodeIdx, DUK_DEFPROP_HAVE_GETTER | DUK_DEFPROP_HAVE_SETTER | DUK_DEFPROP_ENUMERABLE);

        // Attach "classList" property to the prototype
        duk_push_string(ctx, "classList");
        duk_push_c_function(ctx, domNode_classList_get, 0);
        duk_def_prop(ctx, nodeIdx, DUK_DEFPROP_HAVE_GETTER | DUK_DEFPROP_ENUMERABLE);

        // Attach "style" property to the prototype
        duk_push_string(ctx, "style");
        duk_push_c_function(ctx, domNode_style_get, 0);
        duk_def_prop(ctx, nodeIdx, DUK_DEFPROP_HAVE_GETTER | DUK_DEFPROP_ENUMERABLE);

        // Attach "getComputedStyle" function to the prototype
        duk_push_c_function(ctx, domNode_getComputedStyle, 0);
        duk_put_prop_string(ctx, nodeIdx, "getComputedStyle");

        // Attach "setStyle" property to the prototype
        duk_push_c_function(ctx, domNode_setStyle, 2);
        duk_put_prop_string(ctx, nodeIdx, "setStyle");

        // Store the prototype in the stash
        duk_put_prop_string(ctx, -2, DUK_HIDDEN_SYMBOL("DOMNode_prototype"));

        // Pop the global stash
        duk_pop(ctx);
    }

    void push_domNode(duk_context* ctx, DOMNode* node) {
        // Push a DOMNode prototype onto the stack
        push_prototype(ctx, DUK_HIDDEN_SYMBOL("DOMNode_prototype"));

        // Add the DOMNode pointer as property
        duk_push_pointer(ctx, node);
        duk_put_prop_string(ctx, -2, DUK_HIDDEN_SYMBOL("__ptr"));
    }

    void push_domNodeArray(duk_context* ctx, std::vector<DOMNode*> nodes) {
        // Create a new array
        duk_idx_t arrayIdx = duk_push_array(ctx);

        // Loop through the vector of DOMNode pointers
        for (size_t i = 0; i < nodes.size(); i++) {
            // Push the DOMNode object onto the stack
            push_domNode(ctx, nodes[i]);

            // Store the object in the array
            duk_put_prop_index(ctx, arrayIdx, i);
        }
    }

    duk_ret_t domNode_innerXML_get(duk_context* ctx) {
        auto* node = getPtr<DOMNode>(ctx);

        // Push the inner XML to the stack
        if (node) {
            duk_push_string(ctx, node->getInnerXML().c_str());
        } else {
            duk_push_undefined(ctx);
        }

        return 1;
    }

    duk_ret_t domNode_outerXML_get(duk_context* ctx) {
        auto* node = getPtr<DOMNode>(ctx);

        // Push the outer XML to the stack
        if (node) {
            duk_push_string(ctx, node->getOuterXML().c_str());
        } else {
            duk_push_undefined(ctx);
        }

        return 1;
    }

    duk_ret_t domNode_tag_get(duk_context* ctx) {
        auto* node = getPtr<DOMNode>(ctx);

        // Push the tag to the stack
        if (node) {
            duk_push_string(ctx, node->getTag().c_str());
        } else {
            duk_push_undefined(ctx);
        }

        return 1;
    }

    duk_ret_t domNode_name_get(duk_context* ctx) {
        auto* node = getPtr<DOMNode>(ctx);

        // Push the name to the stack
        if (node) {
            duk_push_string(ctx, node->getName().c_str());
        } else {
            duk_push_undefined(ctx);
        }

        return 1;
    }

    duk_ret_t domNode_name_set(duk_context* ctx) {
        auto* node = getPtr<DOMNode>(ctx);

        // Update the name
        if (node) {
            const char* name = duk_get_string(ctx, 0);
            if (name) {
                node->setName(name);
            }
        }

        return 0;
    }

    duk_ret_t domNode_text_get(duk_context* ctx) {
        auto* node = getPtr<DOMNode>(ctx);

        // Push the text to the stack
        if (node) {
            duk_push_string(ctx, (const char*)node->getText().c_str());
        } else {
            duk_push_undefined(ctx);
        }

        return 1;
    }

    duk_ret_t domNode_text_set(duk_context* ctx) {
        auto* node = getPtr<DOMNode>(ctx);

        // Update the text
        if (node) {
            const auto* text = (const char8_t*)duk_get_string(ctx, 0);
            if (text) {
                node->setText(text);
            }
        }

        return 0;
    }

    duk_ret_t domNode_classList_get(duk_context* ctx) {
        auto* node = getPtr<DOMNode>(ctx);

        // Push the classList to the stack
        if (node) {
            push_classList(ctx, node);
        } else {
            duk_push_undefined(ctx);
        }

        return 1;
    }

    duk_ret_t domNode_style_get(duk_context* ctx) {
        auto* node = getPtr<DOMNode>(ctx);

        // Push the name to the stack
        if (node) {
            push_umap_of_string_string(ctx, node->getStyle());
        } else {
            duk_push_undefined(ctx);
        }

        return 1;
    }

    duk_ret_t domNode_getComputedStyle(duk_context* ctx) {
        auto* node = getPtr<DOMNode>(ctx);

        // Push the name to the stack
        if (node) {
            push_umap_of_string_string(ctx, node->getComputedStyle());
        } else {
            duk_push_undefined(ctx);
        }

        return 1;
    }

    duk_ret_t domNode_setStyle(duk_context* ctx) {
        auto* node = getPtr<DOMNode>(ctx);

        // Update the style
        if (node) {
            const char* name = duk_get_string(ctx, 0);
            const char* value = duk_get_string(ctx, 1);
            if (name && value) {
                node->setStyle(name, value);
            }
        }

        return 0;
    }
}