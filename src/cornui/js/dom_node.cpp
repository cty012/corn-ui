#include <corn/ui/ui_manager.h>
#include <cornui/util/exception.h>
#include <cornui/xml/dom.h>
#include <cornui/xml/dom_node.h>
#include "class_list.h"
#include "common.h"
#include "context_data.h"
#include "dom_node.h"

namespace cornui {
    void create_DOMNode(JSContext* ctx) {
        // Create the DOMNode class
        auto* contextData = (ContextData*)JS_GetContextOpaque(ctx);
        JSClassID& classID = contextData->classIDs["DOMNode"];
        JS_NewClassID(&classID);
        JSClassDef classDef = {
            "DOMNode", nullptr, nullptr, nullptr, nullptr
        };
        JS_NewClass(JS_GetRuntime(ctx), classID, &classDef);

        // Create prototype
        JSValue proto = JS_NewObject(ctx);
        JSAtom atom;

        // Attach "innerXML" property
        atom = JS_NewAtom(ctx, "innerXML");
        JS_DefinePropertyGetSet(
                ctx, proto, atom,
                JS_NewCFunction(ctx, js_domNode_innerXML_get, "get", 0),
                JS_NewCFunction(ctx, js_domNode_innerXML_set, "set", 1),
                JS_PROP_ENUMERABLE);
        JS_FreeAtom(ctx, atom);

        // Attach "outerXML" property
        atom = JS_NewAtom(ctx, "outerXML");
        JS_DefinePropertyGetSet(
                ctx, proto, atom,
                JS_NewCFunction(ctx, js_domNode_outerXML_get, "get", 0),
                JS_UNDEFINED,
                JS_PROP_ENUMERABLE);
        JS_FreeAtom(ctx, atom);

        // Attach "toString" function
        JS_SetPropertyStr(
                ctx, proto, "toString",
                JS_NewCFunction(ctx, js_domNode_outerXML_get, "toString", 0));

        // Attach "tag" property
        atom = JS_NewAtom(ctx, "tag");
        JS_DefinePropertyGetSet(
                ctx, proto, atom,
                JS_NewCFunction(ctx, js_domNode_tag_get, "get", 0),
                JS_UNDEFINED,
                JS_PROP_ENUMERABLE);
        JS_FreeAtom(ctx, atom);

        // Attach "name" property
        atom = JS_NewAtom(ctx, "name");
        JS_DefinePropertyGetSet(
                ctx, proto, atom,
                JS_NewCFunction(ctx, js_domNode_name_get, "get", 0),
                JS_NewCFunction(ctx, js_domNode_name_set, "set", 1),
                JS_PROP_ENUMERABLE);
        JS_FreeAtom(ctx, atom);

        // Attach "text" property
        atom = JS_NewAtom(ctx, "text");
        JS_DefinePropertyGetSet(
                ctx, proto, atom,
                JS_NewCFunction(ctx, js_domNode_text_get, "get", 0),
                JS_NewCFunction(ctx, js_domNode_text_set, "set", 1),
                JS_PROP_ENUMERABLE);
        JS_FreeAtom(ctx, atom);

        // Attach "classList" property
        atom = JS_NewAtom(ctx, "classList");
        JS_DefinePropertyGetSet(
                ctx, proto, atom,
                JS_NewCFunction(ctx, js_domNode_classList_get, "get", 0),
                JS_UNDEFINED,
                JS_PROP_ENUMERABLE);
        JS_FreeAtom(ctx, atom);

        // Attach "style" property
        atom = JS_NewAtom(ctx, "style");
        JS_DefinePropertyGetSet(
                ctx, proto, atom,
                JS_NewCFunction(ctx, js_domNode_style_get, "get", 0),
                JS_UNDEFINED,
                JS_PROP_ENUMERABLE);
        JS_FreeAtom(ctx, atom);

        // Attach "computedStyle" property
        atom = JS_NewAtom(ctx, "computedStyle");
        JS_DefinePropertyGetSet(
                ctx, proto, atom,
                JS_NewCFunction(ctx, js_domNode_computedStyle_get, "get", 0),
                JS_UNDEFINED,
                JS_PROP_ENUMERABLE);
        JS_FreeAtom(ctx, atom);

        // Attach "computedGeometry" property
        atom = JS_NewAtom(ctx, "computedGeometry");
        JS_DefinePropertyGetSet(
                ctx, proto, atom,
                JS_NewCFunction(ctx, js_domNode_computedGeometry_get, "get", 0),
                JS_UNDEFINED,
                JS_PROP_ENUMERABLE);
        JS_FreeAtom(ctx, atom);

        // Attach "naturalSize" property
        atom = JS_NewAtom(ctx, "naturalSize");
        JS_DefinePropertyGetSet(
                ctx, proto, atom,
                JS_NewCFunction(ctx, js_domNode_naturalSize_get, "get", 0),
                JS_UNDEFINED,
                JS_PROP_ENUMERABLE);
        JS_FreeAtom(ctx, atom);

        // Attach "setStyle" function
        JS_SetPropertyStr(
                ctx, proto, "setStyle",
                JS_NewCFunction(ctx, js_domNode_setStyle, "setStyle", 2));

        // Attach "animate" function
        JS_SetPropertyStr(
                ctx, proto, "animate",
                JS_NewCFunction(ctx, js_domNode_animate, "animate", 4));

        // Attach "attributes" property
        atom = JS_NewAtom(ctx, "attributes");
        JS_DefinePropertyGetSet(
                ctx, proto, atom,
                JS_NewCFunction(ctx, js_domNode_attributes_get, "get", 0),
                JS_UNDEFINED,
                JS_PROP_ENUMERABLE);
        JS_FreeAtom(ctx, atom);

        // Attach "hasAttribute" function
        JS_SetPropertyStr(
                ctx, proto, "hasAttribute",
                JS_NewCFunction(ctx, js_domNode_hasAttribute, "hasAttribute", 1));

        // Attach "getAttribute" function
        JS_SetPropertyStr(
                ctx, proto, "getAttribute",
                JS_NewCFunction(ctx, js_domNode_getAttribute, "getAttribute", 1));

        // Attach "setAttribute" function
        JS_SetPropertyStr(
                ctx, proto, "setAttribute",
                JS_NewCFunction(ctx, js_domNode_setAttribute, "setAttribute", 2));

        // Attach "removeAttribute" function
        JS_SetPropertyStr(
                ctx, proto, "removeAttribute",
                JS_NewCFunction(ctx, js_domNode_removeAttribute, "removeAttribute", 1));

        // Attach "parent" property
        atom = JS_NewAtom(ctx, "parent");
        JS_DefinePropertyGetSet(
                ctx, proto, atom,
                JS_NewCFunction(ctx, js_domNode_parent_get, "get", 0),
                JS_UNDEFINED,
                JS_PROP_ENUMERABLE);
        JS_FreeAtom(ctx, atom);

        // Attach "children" property
        atom = JS_NewAtom(ctx, "children");
        JS_DefinePropertyGetSet(
                ctx, proto, atom,
                JS_NewCFunction(ctx, js_domNode_children_get, "get", 0),
                JS_UNDEFINED,
                JS_PROP_ENUMERABLE);
        JS_FreeAtom(ctx, atom);

        // Attach "getNodeBySelector" function
        JS_SetPropertyStr(
                ctx, proto, "getNodeBySelector",
                JS_NewCFunction(ctx, js_domNode_getNodeBySelector, "getNodeBySelector", 1));

        // Attach "getNodesBySelector" function
        JS_SetPropertyStr(
                ctx, proto, "getNodesBySelector",
                JS_NewCFunction(ctx, js_domNode_getNodesBySelector, "getNodesBySelector", 1));

        // Attach "focus" function
        JS_SetPropertyStr(
                ctx, proto, "focus",
                JS_NewCFunction(ctx, js_domNode_focus, "focus", 0));

        // Save prototype
        JS_SetClassProto(ctx, classID, proto);
    }

    JSValue js_domNode(JSContext* ctx, DOMNode* node) {
        // Get the class ID
        JSClassID classID;
        if (!getClassID(ctx, &classID, "DOMNode")) {
            return JS_ThrowInternalError(ctx, "DOMNode class is not registered");
        }

        // Create the object
        JSValue obj = JS_NewObjectClass(ctx, (int)classID);
        if (JS_IsException(obj)) {
            JS_FreeValue(ctx, obj);
            return JS_ThrowInternalError(ctx, "Failed to create DOMNode object");
        }
        JS_SetOpaque(obj, node);
        return obj;
    }

    JSValue js_domNodeArray(JSContext* ctx, std::vector<DOMNode*> nodes) {
        // Create a new array
        JSValue array = JS_NewArray(ctx);

        // Add each DOMNode pointer to the array
        for (size_t i = 0; i < nodes.size(); i++) {
            JS_SetPropertyUint32(ctx, array, i, js_domNode(ctx, nodes[i]));
        }

        return array;
    }

    JSValue js_domNode_innerXML_get(JSContext* ctx, JSValueConst this_val, int, JSValueConst*) {
        auto* node = getOpaque<DOMNode>(ctx, this_val, "DOMNode");
        if (!node) {
            return JS_ThrowInternalError(ctx, "DOMNode object is not linked to a DOMNode");
        }

        // Get the inner XML
        return JS_NewString(ctx, node->getInnerXML().c_str());
    }

    JSValue js_domNode_innerXML_set(JSContext* ctx, JSValueConst this_val, int, JSValueConst* argv) {
        auto* node = getOpaque<DOMNode>(ctx, this_val, "DOMNode");
        if (!node) {
            return JS_ThrowInternalError(ctx, "DOMNode object is not linked to a DOMNode");
        }

        // Check the arguments
        if (!JS_IsString(argv[0])) {
            return JS_ThrowTypeError(ctx, "DOMNode.innerXML can only be set to a string");
        }

        // Set the inner XML
        std::string innerXML;
        getString(ctx, &innerXML, argv[0]);
        node->setInnerXML(innerXML);
        node->sync();

        return JS_UNDEFINED;
    }

    JSValue js_domNode_outerXML_get(JSContext* ctx, JSValueConst this_val, int, JSValueConst*) {
        auto* node = getOpaque<DOMNode>(ctx, this_val, "DOMNode");
        if (!node) {
            return JS_ThrowInternalError(ctx, "DOMNode object is not linked to a DOMNode");
        }

        // Get the outer XML
        return JS_NewString(ctx, node->getOuterXML().c_str());
    }

    JSValue js_domNode_tag_get(JSContext* ctx, JSValueConst this_val, int, JSValueConst*) {
        auto* node = getOpaque<DOMNode>(ctx, this_val, "DOMNode");
        if (!node) {
            return JS_ThrowInternalError(ctx, "DOMNode object is not linked to a DOMNode");
        }

        // Get the tag
        return JS_NewString(ctx, node->getTag().c_str());
    }

    JSValue js_domNode_name_get(JSContext* ctx, JSValueConst this_val, int, JSValueConst*) {
        auto* node = getOpaque<DOMNode>(ctx, this_val, "DOMNode");
        if (!node) {
            return JS_ThrowInternalError(ctx, "DOMNode object is not linked to a DOMNode");
        }

        // Get the name
        return JS_NewString(ctx, node->getName().c_str());
    }

    JSValue js_domNode_name_set(JSContext* ctx, JSValueConst this_val, int, JSValueConst* argv) {
        auto* node = getOpaque<DOMNode>(ctx, this_val, "DOMNode");
        if (!node) {
            return JS_ThrowInternalError(ctx, "DOMNode object is not linked to a DOMNode");
        }

        // Check the arguments
        if (!JS_IsString(argv[0])) {
            return JS_ThrowTypeError(ctx, "DOMNode.name can only be set to a string");
        }

        // Set the name
        std::string name;
        getString(ctx, &name, argv[0]);
        node->setName(name);
        node->sync();

        return JS_UNDEFINED;
    }

    JSValue js_domNode_text_get(JSContext* ctx, JSValueConst this_val, int, JSValueConst*) {
        auto* node = getOpaque<DOMNode>(ctx, this_val, "DOMNode");
        if (!node) {
            return JS_ThrowInternalError(ctx, "DOMNode object is not linked to a DOMNode");
        }

        // Get the text
        return JS_NewString(ctx, (const char*)node->getText().c_str());
    }

    JSValue js_domNode_text_set(JSContext* ctx, JSValueConst this_val, int, JSValueConst* argv) {
        auto* node = getOpaque<DOMNode>(ctx, this_val, "DOMNode");
        if (!node) {
            return JS_ThrowInternalError(ctx, "DOMNode object is not linked to a DOMNode");
        }

        // Check the arguments
        if (!JS_IsString(argv[0])) {
            return JS_ThrowTypeError(ctx, "DOMNode.text can only be set to a string");
        }

        // Set the text
        std::string text;
        getString(ctx, &text, argv[0]);
        node->setText((const char8_t*)text.c_str());
        node->sync();

        return JS_UNDEFINED;
    }

    JSValue js_domNode_classList_get(JSContext* ctx, JSValueConst this_val, int, JSValueConst*) {
        auto* node = getOpaque<DOMNode>(ctx, this_val, "DOMNode");
        if (!node) {
            return JS_ThrowInternalError(ctx, "DOMNode object is not linked to a DOMNode");
        }

        // Get the class list
        return js_classList(ctx, node);
    }

    JSValue js_domNode_style_get(JSContext* ctx, JSValueConst this_val, int, JSValueConst*) {
        auto* node = getOpaque<DOMNode>(ctx, this_val, "DOMNode");
        if (!node) {
            return JS_ThrowInternalError(ctx, "DOMNode object is not linked to a DOMNode");
        }

        // Get the style
        return from_njson(ctx, node->getStyle());
    }

    JSValue js_domNode_computedStyle_get(JSContext* ctx, JSValueConst this_val, int, JSValueConst*) {
        auto* node = getOpaque<DOMNode>(ctx, this_val, "DOMNode");
        if (!node) {
            return JS_ThrowInternalError(ctx, "DOMNode object is not linked to a DOMNode");
        }

        // Get the computed style
        return from_njson(ctx, node->getComputedStyle());
    }

    JSValue js_domNode_computedGeometry_get(JSContext* ctx, JSValueConst this_val, int, JSValueConst*) {
        auto* node = getOpaque<DOMNode>(ctx, this_val, "DOMNode");
        if (!node) {
            return JS_ThrowInternalError(ctx, "DOMNode object is not linked to a DOMNode");
        }

        // Check that the node is linked to a widget
        if (!node->getWidget()) {
            return JS_UNDEFINED;
        }
        corn::UIWidget* widget = node->getWidget();

        // Get the cached geometry
        auto [x, y, w, h] = widget->getUIManager().getCachedGeometry(widget);  // NOLINT
        return from_njson(ctx, nlohmann::json::object({
            { "x", x }, { "y", y }, { "w", w }, { "h", h },
        }));
    }

    JSValue js_domNode_naturalSize_get(JSContext* ctx, JSValueConst this_val, int, JSValueConst*) {
        auto* node = getOpaque<DOMNode>(ctx, this_val, "DOMNode");
        if (!node) {
            return JS_ThrowInternalError(ctx, "DOMNode object is not linked to a DOMNode");
        }

        // Check that the node is linked to a widget
        if (!node->getWidget()) {
            return JS_UNDEFINED;
        }
        corn::UIWidget* widget = node->getWidget();

        // Get the natural size
        auto [w, h] = widget->getNaturalSize();  // NOLINT
        return from_njson(ctx, nlohmann::json::object({
            { "w", w }, { "h", h },
        }));
    }

    JSValue js_domNode_setStyle(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv) {
        auto* node = getOpaque<DOMNode>(ctx, this_val, "DOMNode");
        if (!node) {
            return JS_ThrowInternalError(ctx, "DOMNode object is not linked to a DOMNode");
        }

        // Check the arguments
        if (argc != 2) {
            return JS_ThrowTypeError(ctx, "DOMNode.setStyle() expects 2 arguments");
        }
        for (int i = 0; i < argc; i++) {
            if (!JS_IsString(argv[i])) {
                return JS_ThrowTypeError(ctx, "DOMNode.setStyle() expects string arguments");
            }
        }

        // Update the style
        std::string name, value;
        getString(ctx, &name, argv[0]);
        getString(ctx, &value, argv[1]);
        node->setStyle(name, value);
        node->sync();

        return JS_UNDEFINED;
    }

    JSValue js_domNode_animate(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv) {
        auto* node = getOpaque<DOMNode>(ctx, this_val, "DOMNode");
        if (!node) {
            return JS_ThrowInternalError(ctx, "DOMNode object is not linked to a DOMNode");
        }

        // Check the arguments
        if (argc != 4) {
            return JS_ThrowTypeError(ctx, "DOMNode.animate() expects 4 arguments");
        }
        for (int i = 0; i < 3; i++) {
            if (!JS_IsString(argv[i])) {
                return JS_ThrowTypeError(ctx, "DOMNode.animate() expects three string arguments and a number argument");
            }
        }
        if (!JS_IsNumber(argv[3])) {
            return JS_ThrowTypeError(ctx, "DOMNode.animate() expects three string arguments and a number argument");
        }

        // Animate the style
        std::string name, value, type;
        double duration;
        getString(ctx, &name, argv[0]);
        getString(ctx, &value, argv[1]);
        getString(ctx, &type, argv[2]);
        JS_ToFloat64(ctx, &duration, argv[3]);

        // If zero or negative duration, set the style immediately
        if (duration <= 0) {
            node->setStyle(name, value);
            node->sync();
            return JS_UNDEFINED;
        }

        // Otherwise, determine the easing function
        std::unique_ptr<EasingFunction> ease;
        if (type == "linear") {
            ease = std::make_unique<EasingLinear>();
        } else if (type == "step-start") {
            ease = std::make_unique<EasingStepStart>();
        } else if (type == "step-end") {
            ease = std::make_unique<EasingStepEnd>();
        } else if (type == "ease") {
            ease = std::make_unique<EasingEase>();
        } else if (type == "ease-in") {
            ease = std::make_unique<EasingEaseIn>();
        } else if (type == "ease-out") {
            ease = std::make_unique<EasingEaseOut>();
        } else if (type == "ease-in-out") {
            ease = std::make_unique<EasingEaseInOut>();
        } else {
            ease = std::make_unique<EasingEase>();
        }

        // Animate the style
        node->animate(name, value, std::move(ease), (float)duration);

        return JS_UNDEFINED;
    }

    JSValue js_domNode_attributes_get(JSContext* ctx, JSValueConst this_val, int, JSValueConst*) {
        auto* node = getOpaque<DOMNode>(ctx, this_val, "DOMNode");
        if (!node) {
            return JS_ThrowInternalError(ctx, "DOMNode object is not linked to a DOMNode");
        }

        // Get the attributes
        return from_njson(ctx, node->getAttributes());
    }

    JSValue js_domNode_hasAttribute(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv) {
        auto* node = getOpaque<DOMNode>(ctx, this_val, "DOMNode");
        if (!node) {
            return JS_ThrowInternalError(ctx, "DOMNode object is not linked to a DOMNode");
        }

        // Check the arguments
        if (argc != 1) {
            return JS_ThrowTypeError(ctx, "DOMNode.hasAttribute() expects 1 argument");
        }
        if (!JS_IsString(argv[0])) {
            return JS_ThrowTypeError(ctx, "DOMNode.hasAttribute() expects a string argument");
        }

        // Check if the attribute exists
        std::string name;
        getString(ctx, &name, argv[0]);
        return JS_NewBool(ctx, node->getAttributes().contains(name));
    }

    JSValue js_domNode_getAttribute(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv) {
        auto* node = getOpaque<DOMNode>(ctx, this_val, "DOMNode");
        if (!node) {
            return JS_ThrowInternalError(ctx, "DOMNode object is not linked to a DOMNode");
        }

        // Check the arguments
        if (argc != 1) {
            return JS_ThrowTypeError(ctx, "DOMNode.getAttribute() expects 1 argument");
        }
        if (!JS_IsString(argv[0])) {
            return JS_ThrowTypeError(ctx, "DOMNode.getAttribute() expects a string argument");
        }

        // Get the attribute value
        std::string name;
        getString(ctx, &name, argv[0]);
        const std::unordered_map<std::string, std::string>& attrs = node->getAttributes();
        if (attrs.contains(name)) {
            return JS_NewString(ctx, attrs.at(name).c_str());
        } else {
            return JS_UNDEFINED;
        }
    }

    JSValue js_domNode_setAttribute(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv) {
        auto* node = getOpaque<DOMNode>(ctx, this_val, "DOMNode");
        if (!node) {
            return JS_ThrowInternalError(ctx, "DOMNode object is not linked to a DOMNode");
        }

        // Check the arguments
        if (argc != 2) {
            return JS_ThrowTypeError(ctx, "DOMNode.setAttribute() expects 2 arguments");
        }
        for (int i = 0; i < argc; i++) {
            if (!JS_IsString(argv[i])) {
                return JS_ThrowTypeError(ctx, "DOMNode.setAttribute() expects string arguments");
            }
        }

        // Set the attribute
        std::string name, value;
        getString(ctx, &name, argv[0]);
        getString(ctx, &value, argv[1]);
        node->setAttribute(name, value);
        node->sync();

        return JS_UNDEFINED;
    }

    JSValue js_domNode_removeAttribute(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv) {
        auto* node = getOpaque<DOMNode>(ctx, this_val, "DOMNode");
        if (!node) {
            return JS_ThrowInternalError(ctx, "DOMNode object is not linked to a DOMNode");
        }

        // Check the arguments
        if (argc != 1) {
            return JS_ThrowTypeError(ctx, "DOMNode.removeAttribute() expects 1 argument");
        }
        if (!JS_IsString(argv[0])) {
            return JS_ThrowTypeError(ctx, "DOMNode.removeAttribute() expects a string argument");
        }

        // Remove the attribute
        std::string name;
        getString(ctx, &name, argv[0]);
        node->removeAttribute(name);
        node->sync();

        return JS_UNDEFINED;
    }

    JSValue js_domNode_parent_get(JSContext* ctx, JSValueConst this_val, int, JSValueConst*) {
        auto* node = getOpaque<DOMNode>(ctx, this_val, "DOMNode");
        if (!node) {
            return JS_ThrowInternalError(ctx, "DOMNode object is not linked to a DOMNode");
        }

        // Get the parent node
        DOMNode* parent = node->getParent();
        if (parent) {
            return js_domNode(ctx, parent);
        } else {
            return JS_UNDEFINED;
        }
    }

    JSValue js_domNode_children_get(JSContext* ctx, JSValueConst this_val, int, JSValueConst*) {
        auto* node = getOpaque<DOMNode>(ctx, this_val, "DOMNode");
        if (!node) {
            return JS_ThrowInternalError(ctx, "DOMNode object is not linked to a DOMNode");
        }

        // Get the children nodes
        return js_domNodeArray(ctx, node->getChildren());
    }

    JSValue js_domNode_getNodeBySelector(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv) {
        auto* node = getOpaque<DOMNode>(ctx, this_val, "DOMNode");
        if (!node) {
            return JS_ThrowInternalError(ctx, "DOMNode object is not linked to a DOMNode");
        }

        // Check that the node is linked to a DOM
        if (!node->getDOM()) {
            return JS_UNDEFINED;
        }
        DOM* dom = node->getDOM();

        // Check the arguments
        if (argc != 1) {
            return JS_ThrowTypeError(ctx, "DOMNode.getNodeBySelector() expects 1 argument");
        }
        if (!JS_IsString(argv[0])) {
            return JS_ThrowTypeError(ctx, "DOMNode.getNodeBySelector() expects a string argument");
        }

        // Get the node by selector
        std::string selector;
        getString(ctx, &selector, argv[0]);
        try {
            DOMNode* child = dom->getNodeBySelector(selector, node);
            if (child) {
                return js_domNode(ctx, child);
            } else {
                return JS_NULL;
            }
        } catch (const CSSSelectorSyntaxError& e) {
            return JS_NULL;
        }
    }

    JSValue js_domNode_getNodesBySelector(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv) {
        auto* node = getOpaque<DOMNode>(ctx, this_val, "DOMNode");
        if (!node) {
            return JS_ThrowInternalError(ctx, "DOMNode object is not linked to a DOMNode");
        }

        // Check that the node is linked to a DOM
        if (!node->getDOM()) {
            return JS_NULL;
        }
        DOM* dom = node->getDOM();

        // Check the arguments
        if (argc != 1) {
            return JS_ThrowTypeError(ctx, "DOMNode.getNodesBySelector() expects 1 argument");
        }
        if (!JS_IsString(argv[0])) {
            return JS_ThrowTypeError(ctx, "DOMNode.getNodesBySelector() expects a string argument");
        }

        // Get the nodes by selector
        std::string selector;
        getString(ctx, &selector, argv[0]);
        try {
            return js_domNodeArray(ctx, dom->getNodesBySelector(selector, node));
        } catch (const CSSSelectorSyntaxError& e) {
            return JS_NULL;
        }
    }

    JSValue js_domNode_focus(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst*) {
        auto* node = getOpaque<DOMNode>(ctx, this_val, "DOMNode");
        if (!node) {
            return JS_ThrowInternalError(ctx, "DOMNode object is not linked to a DOMNode");
        }

        if (argc != 0) {
            return JS_ThrowTypeError(ctx, "DOMNode.focus() expects 0 arguments");
        }

        // Focus on the node
        node->focus();

        return JS_UNDEFINED;
    }
}
