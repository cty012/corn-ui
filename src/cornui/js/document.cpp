#include <corn/ui/ui_manager.h>
#include <cornui/util/exception.h>
#include <cornui/xml/dom.h>
#include "common.h"
#include "context_data.h"
#include "document.h"
#include "dom_node.h"

namespace cornui {
    void create_document(JSContext* ctx) {
        // Create empty "document" object
        JSValue global = JS_GetGlobalObject(ctx);
        JSValue document = JS_NewObject(ctx);

        JSAtom atom;

        // Attach "root" property
        atom = JS_NewAtom(ctx, "root");
        JS_DefinePropertyGetSet(
                ctx, document, atom,
                JS_NewCFunction(ctx, js_document_root_get, "get", 0),
                JS_UNDEFINED,
                JS_PROP_ENUMERABLE);
        JS_FreeAtom(ctx, atom);

        // Attach "getNodeBySelector" function
        JS_SetPropertyStr(
                ctx, document, "getNodeBySelector",
                JS_NewCFunction(ctx, js_document_getNodeBySelector, "getNodeBySelector", 1));

        // Attach "getNodesBySelector" function
        JS_SetPropertyStr(
                ctx, document, "getNodesBySelector",
                JS_NewCFunction(ctx, js_document_getNodesBySelector, "getNodesBySelector", 1));

        // Attach "focusedNode" property
        atom = JS_NewAtom(ctx, "focusedNode");
        JS_DefinePropertyGetSet(
                ctx, document, atom,
                JS_NewCFunction(ctx, js_document_focusedNode_get, "get", 0),
                JS_UNDEFINED,
                JS_PROP_ENUMERABLE);
        JS_FreeAtom(ctx, atom);

        // Attach "document" to the global object
        JS_SetPropertyStr(ctx, global, "document", document);
        JS_FreeValue(ctx, global);
    }

    JSValue js_document_root_get(JSContext* ctx, JSValueConst, int, JSValueConst*) {
        auto* contextData = (ContextData*)JS_GetContextOpaque(ctx);

        // Get the root node
        return js_domNode(ctx, &contextData->dom->getRoot());
    }

    JSValue js_document_getNodeBySelector(JSContext* ctx, JSValueConst, int argc, JSValueConst* argv) {
        auto* contextData = (ContextData*)JS_GetContextOpaque(ctx);

        // Check the arguments
        if (argc != 1) {
            return JS_ThrowTypeError(ctx, "document.getNodeBySelector() expects 1 argument");
        }
        if (!JS_IsString(argv[0])) {
            return JS_ThrowTypeError(ctx, "document.getNodeBySelector() expects a string argument");
        }

        // Get the node by selector
        std::string selector;
        getString(ctx, &selector, argv[0]);
        try {
            DOMNode* child = contextData->dom->getNodeBySelector(selector);
            if (child) {
                return js_domNode(ctx, child);
            } else {
                return JS_NULL;
            }
        } catch (const CSSSelectorSyntaxError& e) {
            return JS_NULL;
        }
    }

    JSValue js_document_getNodesBySelector(JSContext* ctx, JSValueConst, int argc, JSValueConst* argv) {
        auto* contextData = (ContextData*)JS_GetContextOpaque(ctx);

        // Check the arguments
        if (argc != 1) {
            return JS_ThrowTypeError(ctx, "document.getNodesBySelector() expects 1 argument");
        }
        if (!JS_IsString(argv[0])) {
            return JS_ThrowTypeError(ctx, "document.getNodesBySelector() expects a string argument");
        }

        // Get the nodes by selector
        std::string selector;
        getString(ctx, &selector, argv[0]);
        try {
            return js_domNodeArray(ctx, contextData->dom->getNodesBySelector(selector));
        } catch (const CSSSelectorSyntaxError& e) {
            return JS_NULL;
        }
    }

    JSValue js_document_focusedNode_get(JSContext* ctx, JSValueConst, int, JSValueConst*) {
        auto* contextData = (ContextData*)JS_GetContextOpaque(ctx);

        // Get the focused node
        return js_domNode(ctx, contextData->dom->getFocusedNode());
    }
}
