#include <cornui/util/exception.h>
#include <cornui/xml/dom.h>
#include "common.h"
#include "document.h"
#include "dom_node.h"

namespace cornui {
    void create_document(duk_context* ctx, DOM* dom) {
        // Push the global object and "document" object onto the stack
        duk_push_global_object(ctx);
        duk_idx_t documentIdx = duk_push_object(ctx);

        // Add the DOM pointer as hidden property
        duk_push_pointer(ctx, dom);
        duk_put_prop_string(ctx, documentIdx, DUK_HIDDEN_SYMBOL("__ptr"));

        // Add property "root" to "document"
        duk_push_string(ctx, "root");
        duk_push_c_function(ctx, document_root_get, 0);
        duk_def_prop(ctx, documentIdx, DUK_DEFPROP_HAVE_GETTER | DUK_DEFPROP_ENUMERABLE);

        // Add function "getNodeBySelector" to "document"
        duk_push_c_function(ctx, document_getNodeBySelector, 1);
        duk_put_prop_string(ctx, documentIdx, "getNodeBySelector");

        // Add function "getNodesBySelector" to "document"
        duk_push_c_function(ctx, document_getNodesBySelector, 1);
        duk_put_prop_string(ctx, documentIdx, "getNodesBySelector");

        // Add property "focusedNode" to "document"
        duk_push_string(ctx, "focusedNode");
        duk_push_c_function(ctx, document_focusedNode_get, 0);
        duk_def_prop(ctx, documentIdx, DUK_DEFPROP_HAVE_GETTER | DUK_DEFPROP_ENUMERABLE);

        // Add "document" to the global object
        duk_put_global_string(ctx, "document");

        // Pop the global object
        duk_pop(ctx);
    }

    duk_ret_t document_root_get(duk_context* ctx) {
        // Get the DOM pointer
        auto* dom = getPtr<DOM>(ctx);

        // Push result to stack
        if (dom) {
            push_domNode(ctx, &dom->getRoot());
        } else {
            duk_push_undefined(ctx);
        }

        return 1;
    }

    duk_ret_t document_getNodeBySelector(duk_context* ctx) {
        // Get the DOM pointer
        auto* dom = getPtr<DOM>(ctx);

        // Push result to stack
        if (dom) {
            try {
                std::string selector = duk_get_string(ctx, 0);
                push_domNode(ctx, dom->getNodeBySelector(selector));
            } catch (const CSSSelectorSyntaxError& e) {
                duk_push_null(ctx);
            }
        } else {
            duk_push_undefined(ctx);
        }

        return 1;
    }

    duk_ret_t document_getNodesBySelector(duk_context* ctx) {
        // Get the DOM pointer
        auto* dom = getPtr<DOM>(ctx);

        // Push result to stack
        if (dom) {
            try {
                std::string selector = duk_get_string(ctx, 0);
                push_domNodeArray(ctx, dom->getNodesBySelector(selector));
            } catch (const CSSSelectorSyntaxError& e) {
                duk_push_array(ctx);
            }
        } else {
            duk_push_undefined(ctx);
        }

        return 1;
    }

    duk_ret_t document_focusedNode_get(duk_context* ctx) {
        // Get the DOM pointer
        auto* dom = getPtr<DOM>(ctx);

        // Push result to stack
        if (dom && dom->getUIManager()) {
            corn::UIWidget* focusedWidget = dom->getUIManager()->getFocusedWidget();
            DOMNode* focusedNode = dom->getNodeThat([focusedWidget](const DOMNode* node) {
                return node->getWidget() == focusedWidget;
            });
            if (focusedNode) {
                push_domNode(ctx, focusedNode);
            } else {
                duk_push_null(ctx);
            }
        } else {
            duk_push_undefined(ctx);
        }

        return 1;
    }
}
