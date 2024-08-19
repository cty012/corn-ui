#include <sstream>
#include <cornui/xml/dom_node.h>
#include "class_list.h"
#include "context_data.h"
#include "common.h"

namespace cornui {
    void create_ClassList(JSContext* ctx) {
        // Create the ClassList class
        auto* contextData = (ContextData*)JS_GetContextOpaque(ctx);
        JSClassID& classID = contextData->classIDs["ClassList"];
        JS_NewClassID(&classID);
        JSClassDef classDef = {
            "ClassList", nullptr, nullptr, nullptr, nullptr
        };
        JS_NewClass(JS_GetRuntime(ctx), classID, &classDef);

        // Create prototype
        JSValue proto = JS_NewObject(ctx);

        // Attach "toArray" function
        JS_SetPropertyStr(
                ctx, proto, "toArray",
                JS_NewCFunction(ctx, js_classList_toArray, "toArray", 0));

        // Attach "toString" function
        JS_SetPropertyStr(
                ctx, proto, "toString",
                JS_NewCFunction(ctx, js_classList_toString, "toString", 0));

        // Attach "contains" function
        JS_SetPropertyStr(
                ctx, proto, "contains",
                JS_NewCFunction(ctx, js_classList_contains, "contains", 1));

        // Attach "item" function
        JS_SetPropertyStr(
                ctx, proto, "item",
                JS_NewCFunction(ctx, js_classList_item, "item", 1));

        // Attach "add" function
        JS_SetPropertyStr(
                ctx, proto, "add",
                JS_NewCFunction(ctx, js_classList_add, "add", 1));

        // Attach "remove" function
        JS_SetPropertyStr(
                ctx, proto, "remove",
                JS_NewCFunction(ctx, js_classList_remove, "remove", 1));

        // Attach "toggle" function
        JS_SetPropertyStr(
                ctx, proto, "toggle",
                JS_NewCFunction(ctx, js_classList_toggle, "toggle", 1));

        // Attach "replace" function
        JS_SetPropertyStr(
                ctx, proto, "replace",
                JS_NewCFunction(ctx, js_classList_replace, "replace", 2));

        // Save prototype
        JS_SetClassProto(ctx, classID, proto);
    }

    JSValue js_classList(JSContext* ctx, DOMNode* node) {
        // Get the class ID
        JSClassID classID;
        if (!getClassID(ctx, &classID, "ClassList")) {
            return JS_ThrowInternalError(ctx, "ClassList class is not registered");
        }

        // Create the object
        JSValue obj = JS_NewObjectClass(ctx, (int)classID);
        if (JS_IsException(obj)) {
            JS_FreeValue(ctx, obj);
            return JS_ThrowInternalError(ctx, "Failed to create CLassList object");
        }
        JS_SetOpaque(obj, node);
        return obj;
    }

    JSValue js_classList_toArray(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst*) {
        auto* node = getOpaque<DOMNode>(ctx, this_val, "ClassList");
        if (!node) {
            return JS_ThrowInternalError(ctx, "ClassList object is not linked to a DOMNode");
        }

        if (argc != 0) {
            return JS_ThrowTypeError(ctx, "ClassList.toArray() expects no arguments");
        }

        return from_njson(ctx, node->getClassList());
    }

    JSValue js_classList_toString(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst*) {
        auto* node = getOpaque<DOMNode>(ctx, this_val, "ClassList");
        if (!node) {
            return JS_ThrowInternalError(ctx, "ClassList object is not linked to a DOMNode");
        }

        if (argc != 0) {
            return JS_ThrowTypeError(ctx, "ClassList.toString() expects no arguments");
        }

        const std::vector<std::string>& classList = node->getClassList();
        std::stringstream classNames;
        for (size_t i = 0; i < classList.size(); i++) {
            if (i) classNames << " ";
            classNames << classList[i];
        }
        return JS_NewString(ctx, classNames.str().c_str());
    }

    JSValue js_classList_contains(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv) {
        auto* node = getOpaque<DOMNode>(ctx, this_val, "ClassList");
        if (!node) {
            return JS_ThrowInternalError(ctx, "ClassList object is not linked to a DOMNode");
        }

        if (argc == 0) {
            return JS_ThrowTypeError(ctx, "ClassList.contains() expects at least 1 argument");
        }

        bool contains = true;
        for (int i = 0; i < argc; i++) {
            // Check that argument is a string
            if (!JS_IsString(argv[i])) {
                return JS_ThrowTypeError(ctx, "ClassList.contains() expects string arguments");
            }

            // Get the class name
            std::string className;
            getString(ctx, &className, argv[i]);

            // Check if the class is in the class list
            if (!node->hasClass(className)) {
                contains = false;
            }
        }

        if (contains) {
            return JS_TRUE;
        } else {
            return JS_FALSE;
        }
    }

    JSValue js_classList_item(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv) {
        auto* node = getOpaque<DOMNode>(ctx, this_val, "ClassList");
        if (!node) {
            return JS_ThrowInternalError(ctx, "ClassList object is not linked to a DOMNode");
        }

        if (argc != 1) {
            return JS_ThrowTypeError(ctx, "ClassList.item() expects at least 1 argument");
        }

        // Check that the argument is a number
        if (!JS_IsNumber(argv[0])) {
            return JS_ThrowTypeError(ctx, "ClassList.item() expects a non-negative integer argument");
        }

        // Check that the argument is a non-negative integer
        int64_t index = 0;
        double dIndex = 0;
        if (JS_ToInt64(ctx, &index, argv[0])) {
            return JS_ThrowTypeError(ctx, "ClassList.item() expects a non-negative integer argument");
        }
        JS_ToFloat64(ctx, &dIndex, argv[0]);
        if (dIndex != (double)index) {
            return JS_ThrowTypeError(ctx, "ClassList.item() expects a non-negative integer argument");
        }

        // Check that index is in range
        const std::vector<std::string>& classList = node->getClassList();
        if (index < 0 || (size_t)index >= classList.size()) {
            return JS_UNDEFINED;
        }

        return JS_NewString(ctx, classList[index].c_str());
    }

    JSValue js_classList_add(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv) {
        auto* node = getOpaque<DOMNode>(ctx, this_val, "ClassList");
        if (!node) {
            return JS_ThrowInternalError(ctx, "ClassList object is not linked to a DOMNode");
        }

        if (argc == 0) {
            return JS_ThrowTypeError(ctx, "ClassList.add() expects at least 1 argument");
        }

        // Validate arguments
        for (int i = 0; i < argc; i++) {
            if (!JS_IsString(argv[i])) {
                return JS_ThrowTypeError(ctx, "ClassList.add() expects string arguments");
            }
        }

        // Add each class
        bool altered = false;
        for (int i = 0; i < argc; i++) {
            std::string className;
            getString(ctx, &className, argv[i]);

            if (node->addClass(className)) {
                altered = true;
            }
        }

        // Sync the DOM node
        if (altered) node->sync();

        return JS_UNDEFINED;
    }

    JSValue js_classList_remove(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv) {
        auto* node = getOpaque<DOMNode>(ctx, this_val, "ClassList");
        if (!node) {
            return JS_ThrowInternalError(ctx, "ClassList object is not linked to a DOMNode");
        }

        if (argc == 0) {
            return JS_ThrowTypeError(ctx, "ClassList.remove() expects at least 1 argument");
        }

        // Validate arguments
        for (int i = 0; i < argc; i++) {
            if (!JS_IsString(argv[i])) {
                return JS_ThrowTypeError(ctx, "ClassList.remove() expects string arguments");
            }
        }

        // Remove each class
        bool altered = false;
        for (int i = 0; i < argc; i++) {
            std::string className;
            getString(ctx, &className, argv[i]);

            if (node->removeClass(className)) {
                altered = true;
            }
        }

        // Sync the DOM node
        if (altered) node->sync();

        return JS_UNDEFINED;
    }

    JSValue js_classList_toggle(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv) {
        auto* node = getOpaque<DOMNode>(ctx, this_val, "ClassList");
        if (!node) {
            return JS_ThrowInternalError(ctx, "ClassList object is not linked to a DOMNode");
        }

        if (argc == 0) {
            return JS_ThrowTypeError(ctx, "ClassList.toggle() expects at least 1 argument");
        }

        // Validate arguments
        for (int i = 0; i < argc; i++) {
            if (!JS_IsString(argv[i])) {
                return JS_ThrowTypeError(ctx, "ClassList.toggle() expects string arguments");
            }
        }

        // Toggle each class
        for (int i = 0; i < argc; i++) {
            std::string className;
            getString(ctx, &className, argv[i]);

            if (node->hasClass(className)) {
                node->removeClass(className);
            } else {
                node->addClass(className);
            }
        }

        // Sync the DOM node
        node->sync();

        return JS_UNDEFINED;
    }

    JSValue js_classList_replace(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv) {
        auto* node = getOpaque<DOMNode>(ctx, this_val, "ClassList");
        if (!node) {
            return JS_ThrowInternalError(ctx, "ClassList object is not linked to a DOMNode");
        }

        if (argc != 2) {
            return JS_ThrowTypeError(ctx, "ClassList.replace() expects 2 arguments");
        }

        // Validate arguments
        for (int i = 0; i < argc; i++) {
            if (!JS_IsString(argv[i])) {
                return JS_ThrowTypeError(ctx, "ClassList.replace() expects string arguments");
            }
        }

        // Replace the class
        std::string oldClassName, newClassName;
        getString(ctx, &oldClassName, argv[0]);
        getString(ctx, &newClassName, argv[1]);

        if (node->hasClass(oldClassName)) {
            node->removeClass(oldClassName);
            node->addClass(newClassName);
            node->sync();
        }

        return JS_UNDEFINED;
    }
}
