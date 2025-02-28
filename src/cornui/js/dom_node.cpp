#include <corn/ui/ui_manager.h>
#include <cornui/util/exception.h>
#include <cornui/xml/dom.h>
#include <cornui/xml/dom_node.h>
#include "class_list.h"
#include "common.h"
#include "dom_node.h"

namespace cornui {
    void create_domNode_prototype(duk_context* ctx) {
        // Push the global stash and the prototype object onto the stack
        duk_push_global_stash(ctx);
        const duk_idx_t nodeIdx = duk_push_object(ctx);

        // Attach "innerXML" property to the prototype
        duk_push_string(ctx, "innerXML");
        duk_push_c_function(ctx, domNode_innerXML_get, 0);
        duk_push_c_function(ctx, domNode_innerXML_set, 1);
        duk_def_prop(ctx, nodeIdx, DUK_DEFPROP_HAVE_GETTER | DUK_DEFPROP_HAVE_SETTER | DUK_DEFPROP_ENUMERABLE);

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

        // Attach "computedStyle" property to the prototype
        duk_push_string(ctx, "computedStyle");
        duk_push_c_function(ctx, domNode_computedStyle_get, 0);
        duk_def_prop(ctx, nodeIdx, DUK_DEFPROP_HAVE_GETTER | DUK_DEFPROP_ENUMERABLE);

        // Attach "computedGeometry" property to the prototype
        duk_push_string(ctx, "computedGeometry");
        duk_push_c_function(ctx, domNode_computedGeometry_get, 0);
        duk_def_prop(ctx, nodeIdx, DUK_DEFPROP_HAVE_GETTER | DUK_DEFPROP_ENUMERABLE);

        // Attach "naturalSize" property to the prototype
        duk_push_string(ctx, "naturalSize");
        duk_push_c_function(ctx, domNode_naturalSize_get, 0);
        duk_def_prop(ctx, nodeIdx, DUK_DEFPROP_HAVE_GETTER | DUK_DEFPROP_ENUMERABLE);

        // Attach "setStyle" function to the prototype
        duk_push_c_function(ctx, domNode_setStyle, 2);
        duk_put_prop_string(ctx, nodeIdx, "setStyle");

        // Attach "animate" function to the prototype
        duk_push_c_function(ctx, domNode_animate, 4);
        duk_put_prop_string(ctx, nodeIdx, "animate");

        // Attach "attributes" property to the prototype
        duk_push_string(ctx, "attributes");
        duk_push_c_function(ctx, domNode_attributes_get, 0);
        duk_def_prop(ctx, nodeIdx, DUK_DEFPROP_HAVE_GETTER | DUK_DEFPROP_ENUMERABLE);

        // Attach "hasAttribute" function to the prototype
        duk_push_c_function(ctx, domNode_hasAttribute, 1);
        duk_put_prop_string(ctx, nodeIdx, "hasAttribute");

        // Attach "getAttribute" function to the prototype
        duk_push_c_function(ctx, domNode_getAttribute, 1);
        duk_put_prop_string(ctx, nodeIdx, "getAttribute");

        // Attach "setAttribute" function to the prototype
        duk_push_c_function(ctx, domNode_setAttribute, 2);
        duk_put_prop_string(ctx, nodeIdx, "setAttribute");

        // Attach "removeAttribute" function to the prototype
        duk_push_c_function(ctx, domNode_removeAttribute, 1);
        duk_put_prop_string(ctx, nodeIdx, "removeAttribute");

        // Attach "parent" property to the prototype
        duk_push_string(ctx, "parent");
        duk_push_c_function(ctx, domNode_parent_get, 0);
        duk_def_prop(ctx, nodeIdx, DUK_DEFPROP_HAVE_GETTER | DUK_DEFPROP_ENUMERABLE);

        // Attach "children" property to the prototype
        duk_push_string(ctx, "children");
        duk_push_c_function(ctx, domNode_children_get, 0);
        duk_def_prop(ctx, nodeIdx, DUK_DEFPROP_HAVE_GETTER | DUK_DEFPROP_ENUMERABLE);

        // Attach "getNodeBySelector" function to the prototype
        duk_push_c_function(ctx, domNode_getNodeBySelector, 1);
        duk_put_prop_string(ctx, nodeIdx, "getNodeBySelector");

        // Attach "getNodesBySelector" function to the prototype
        duk_push_c_function(ctx, domNode_getNodesBySelector, 1);
        duk_put_prop_string(ctx, nodeIdx, "getNodesBySelector");

        // Attach "focus" function to the prototype
        duk_push_c_function(ctx, domNode_focus, 0);
        duk_put_prop_string(ctx, nodeIdx, "focus");

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
        const duk_idx_t arrayIdx = duk_push_array(ctx);

        // Loop through the vector of DOMNode pointers
        for (size_t i = 0; i < nodes.size(); i++) {
            // Push the DOMNode object onto the stack
            push_domNode(ctx, nodes[i]);

            // Store the object in the array
            duk_put_prop_index(ctx, arrayIdx, static_cast<duk_uarridx_t>(i));
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

    duk_ret_t domNode_innerXML_set(duk_context* ctx) {
        auto* node = getPtr<DOMNode>(ctx);

        // Push the inner XML to the stack
        if (node) {
            const char* innerXML = duk_get_string(ctx, 0);
            if (innerXML) {
                node->setInnerXML(innerXML);
                node->sync();
            }
        }

        return 0;
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
                node->sync();
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
                node->sync();
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
            push_njson(ctx, node->getStyle());
        } else {
            duk_push_undefined(ctx);
        }

        return 1;
    }

    duk_ret_t domNode_computedStyle_get(duk_context* ctx) {
        auto* node = getPtr<DOMNode>(ctx);

        // Push the name to the stack
        if (node) {
            push_njson(ctx, node->getComputedStyle());
        } else {
            duk_push_undefined(ctx);
        }

        return 1;
    }

    duk_ret_t domNode_computedGeometry_get(duk_context* ctx) {
        auto* node = getPtr<DOMNode>(ctx);

        // Push the cached geometry to the stack
        if (node && node->getWidget()) {
            corn::UIWidget* widget = node->getWidget();
            auto [x, y, w, h] = widget->getUIManager().getCachedGeometry(widget);  // NOLINT
            push_njson(ctx, {{ "x", x }, { "y", y }, { "w", w }, { "h", h }});
        } else {
            duk_push_undefined(ctx);
        }

        return 1;
    }

    duk_ret_t domNode_naturalSize_get(duk_context* ctx) {
        const auto* node = getPtr<DOMNode>(ctx);

        // Push the cached geometry to the stack
        if (node && node->getWidget()) {
            const corn::UIWidget* widget = node->getWidget();
            auto [w, h] = widget->getNaturalSize();  // NOLINT
            push_njson(ctx, {{ "w", w }, { "h", h }});
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
                node->sync();
            }
        }

        return 0;
    }

    duk_ret_t domNode_animate(duk_context* ctx) {
        auto* node = getPtr<DOMNode>(ctx);

        // Animate the style
        if (node) {
            const char* name = duk_get_string(ctx, 0);
            const char* value = duk_get_string(ctx, 1);
            const char* typeStr = duk_get_string(ctx, 2);
            const auto duration = static_cast<float>(duk_get_number(ctx, 3));

            // Validate input
            if (!name || !value || !typeStr) {
                return 0;
            }

            if (duration <= 0) {
                node->setStyle(name, value);
                node->sync();
                return 0;
            }

            std::unique_ptr<EasingFunction> ease;
            if (strcmp(typeStr, "linear") == 0) {
                ease = std::make_unique<EasingLinear>();
            } else if (strcmp(typeStr, "step-start") == 0) {
                ease = std::make_unique<EasingStepStart>();
            } else if (strcmp(typeStr, "step-end") == 0) {
                ease = std::make_unique<EasingStepEnd>();
            } else if (strcmp(typeStr, "ease") == 0) {
                ease = std::make_unique<EasingEase>();
            } else if (strcmp(typeStr, "ease-in") == 0) {
                ease = std::make_unique<EasingEaseIn>();
            } else if (strcmp(typeStr, "ease-out") == 0) {
                ease = std::make_unique<EasingEaseOut>();
            } else if (strcmp(typeStr, "ease-in-out") == 0) {
                ease = std::make_unique<EasingEaseInOut>();
            } else {
                ease = std::make_unique<EasingEase>();
            }

            node->animate(name, value, std::move(ease), duration);
        }

        return 0;
    }

    duk_ret_t domNode_attributes_get(duk_context* ctx) {
        const auto* node = getPtr<DOMNode>(ctx);

        // Push the name to the stack
        if (node) {
            push_njson(ctx, node->getAttributes());
        } else {
            duk_push_undefined(ctx);
        }

        return 1;
    }

    duk_ret_t domNode_hasAttribute(duk_context* ctx) {
        const auto* node = getPtr<DOMNode>(ctx);

        // Push the name to the stack
        if (node) {
            const std::unordered_map<std::string, std::string>& attrs = node->getAttributes();
            const char* name = duk_get_string(ctx, 0);
            if (name) {
                duk_push_boolean(ctx, attrs.contains(name));
            } else {
                duk_push_undefined(ctx);
            }
        } else {
            duk_push_undefined(ctx);
        }

        return 1;
    }

    duk_ret_t domNode_getAttribute(duk_context* ctx) {
        const auto* node = getPtr<DOMNode>(ctx);

        // Push the name to the stack
        if (node) {
            const std::unordered_map<std::string, std::string>& attrs = node->getAttributes();
            const char* name = duk_get_string(ctx, 0);
            if (name && attrs.contains(name)) {
                duk_push_string(ctx, attrs.at(name).c_str());
            } else {
                duk_push_undefined(ctx);
            }
        } else {
            duk_push_undefined(ctx);
        }

        return 1;
    }

    duk_ret_t domNode_setAttribute(duk_context* ctx) {
        auto* node = getPtr<DOMNode>(ctx);

        // Push the name to the stack
        if (node) {
            const char* name = duk_get_string(ctx, 0);
            const char* value = duk_get_string(ctx, 1);
            if (name && value) {
                node->setAttribute(name, value);
                node->sync();
            }
        }

        return 0;
    }

    duk_ret_t domNode_removeAttribute(duk_context* ctx) {
        auto* node = getPtr<DOMNode>(ctx);

        // Push the name to the stack
        if (node) {
            const char* name = duk_get_string(ctx, 0);
            if (name) {
                node->removeAttribute(name);
                node->sync();
            }
        }

        return 0;
    }

    duk_ret_t domNode_parent_get(duk_context* ctx) {
        const auto* node = getPtr<DOMNode>(ctx);

        // Push the parent nodes to the stack
        if (node) {
            DOMNode* parent = node->getParent();
            if (parent) {
                push_domNode(ctx, parent);
            } else {
                duk_push_undefined(ctx);
            }
        } else {
            duk_push_undefined(ctx);
        }

        return 1;
    }

    duk_ret_t domNode_children_get(duk_context* ctx) {
        const auto* node = getPtr<DOMNode>(ctx);

        // Push the child nodes to the stack
        if (node) {
            const std::vector<DOMNode*> children = node->getChildren();
            push_domNodeArray(ctx, children);
        } else {
            duk_push_undefined(ctx);
        }

        return 1;
    }

    duk_ret_t domNode_getNodeBySelector(duk_context* ctx) {
        const auto* node = getPtr<DOMNode>(ctx);

        // Push the node to stack
        if (node) {
            try {
                const char* selector = duk_get_string(ctx, 0);
                push_domNode(ctx, node->getDOM()->getNodeBySelector(selector, node));
            } catch (const CSSSelectorSyntaxError&) {
                duk_push_null(ctx);
            }
        } else {
            duk_push_undefined(ctx);
        }

        return 1;
    }

    duk_ret_t domNode_getNodesBySelector(duk_context* ctx) {
        const auto* node = getPtr<DOMNode>(ctx);

        // Push the node to stack
        if (node) {
            try {
                const char* selector = duk_get_string(ctx, 0);
                push_domNodeArray(ctx, node->getDOM()->getNodesBySelector(selector, node));
            } catch (const CSSSelectorSyntaxError&) {
                duk_push_array(ctx);
            }
        } else {
            duk_push_undefined(ctx);
        }

        return 1;
    }

    // todo: Create API in DOMNode to focus on the node
    duk_ret_t domNode_focus(duk_context* ctx) {
        const auto* node = getPtr<DOMNode>(ctx);

        // Focus on the node
        if (node && node->getWidget()) {
            corn::UIWidget* widget = node->getWidget();
            widget->getUIManager().setFocusedWidget(widget);
        }

        return 0;
    }
}
