#include <sstream>
#include <cornui/xml/dom_node.h>
#include "class_list.h"
#include "common.h"

namespace cornui {
    void create_classList_prototype(duk_context* ctx) {
        // Push the global stash and the prototype object onto the stack
        duk_push_global_stash(ctx);
        duk_idx_t nodeIdx = duk_push_object(ctx);

        // Attach "toArray" function to the prototype
        duk_push_c_function(ctx, classList_toArray, 0);
        duk_put_prop_string(ctx, nodeIdx, "toArray");

        // Attach "toString" function to the prototype
        duk_push_c_function(ctx, classList_toString, 0);
        duk_put_prop_string(ctx, nodeIdx, "toString");

        // Attach "contains" function to the prototype
        duk_push_c_function(ctx, classList_contains, 1);
        duk_put_prop_string(ctx, nodeIdx, "contains");

        // Attach "item" function to the prototype
        duk_push_c_function(ctx, classList_item, 1);
        duk_put_prop_string(ctx, nodeIdx, "item");

        // Attach "add" function to the prototype
        duk_push_c_function(ctx, classList_add, DUK_VARARGS);
        duk_put_prop_string(ctx, nodeIdx, "add");

        // Attach "remove" function to the prototype
        duk_push_c_function(ctx, classList_remove, DUK_VARARGS);
        duk_put_prop_string(ctx, nodeIdx, "remove");

        // Attach "toggle" function to the prototype
        duk_push_c_function(ctx, classList_toggle, 1);
        duk_put_prop_string(ctx, nodeIdx, "toggle");

        // Attach "replace" function to the prototype
        duk_push_c_function(ctx, classList_replace, 2);
        duk_put_prop_string(ctx, nodeIdx, "replace");

        // Store the prototype in the stash
        duk_put_prop_string(ctx, -2, DUK_HIDDEN_SYMBOL("classList_prototype"));

        // Pop the global stash
        duk_pop(ctx);
    }

    void push_classList(duk_context* ctx, DOMNode* node) {
        // Push a DOMNode prototype onto the stack
        push_prototype(ctx, DUK_HIDDEN_SYMBOL("classList_prototype"));

        // Add the DOMNode pointer as property
        duk_push_pointer(ctx, node);
        duk_put_prop_string(ctx, -2, DUK_HIDDEN_SYMBOL("__ptr"));
    }

    duk_ret_t classList_toArray(duk_context* ctx) {
        const auto* node = getPtr<DOMNode>(ctx);

        // Push the outer XML to the stack
        if (node) {
            push_njson(ctx, node->getClassList());
        } else {
            duk_push_undefined(ctx);
        }

        return 1;
    }

    duk_ret_t classList_toString(duk_context* ctx) {
        const auto* node = getPtr<DOMNode>(ctx);

        // Push the outer XML to the stack
        if (node) {
            const std::vector<std::string>& classList = node->getClassList();
            std::stringstream classNames;
            for (size_t i = 0; i < classList.size(); i++) {
                if (i) classNames << " ";
                classNames << classList[i];
            }
            duk_push_string(ctx, classNames.str().c_str());
        } else {
            duk_push_undefined(ctx);
        }

        return 1;
    }

    duk_ret_t classList_contains(duk_context* ctx) {
        const auto* node = getPtr<DOMNode>(ctx);
        const char* className = duk_get_string(ctx, 0);

        if (node && className) {
            duk_push_boolean(ctx, node->hasClass(className));
        } else {
            duk_push_undefined(ctx);
        }

        return 1;
    }

    duk_ret_t classList_item(duk_context* ctx) {
        // Check that node is valid
        const auto* node = getPtr<DOMNode>(ctx);
        if (!node) {
            duk_push_undefined(ctx);
            return 1;
        }

        // Check that arguments are valid
        if (!duk_is_number(ctx, 0)) {
            duk_push_undefined(ctx);
            return 1;
        }
        const double num = duk_get_number(ctx, 0);
        const int index = duk_get_int(ctx, 0);
        if (static_cast<const double>(index) == num) {
            duk_push_undefined(ctx);
            return 1;
        }

        // Check that index is in range
        const std::vector<std::string>& classList = node->getClassList();
        if (index < 0 || static_cast<size_t>(index) >= classList.size()) {
            duk_push_undefined(ctx);
            return 1;
        }

        duk_push_string(ctx, classList[index].c_str());
        return 1;
    }

    duk_ret_t classList_add(duk_context* ctx) {
        const duk_idx_t nargs = duk_get_top(ctx);
        auto* node = getPtr<DOMNode>(ctx);

        // Add each class
        if (node) {
            bool altered = false;
            for (int i = 0; i < nargs; i++) {
                const char* className = duk_get_string(ctx, i);
                if (className) {
                    node->addClass(className);
                    altered = true;
                }
            }
            if (altered) node->sync();
        }

        return 0;
    }

    duk_ret_t classList_remove(duk_context* ctx) {
        const duk_idx_t nargs = duk_get_top(ctx);
        auto* node = getPtr<DOMNode>(ctx);

        // Remove each class
        if (node) {
            bool altered = false;
            for (int i = 0; i < nargs; i++) {
                const char* className = duk_get_string(ctx, i);
                if (className) {
                    node->removeClass(className);
                    altered = true;
                }
            }
            if (altered) node->sync();
        }

        return 0;
    }

    duk_ret_t classList_toggle(duk_context* ctx) {
        auto* node = getPtr<DOMNode>(ctx);

        // Toggle each class
        if (node) {
            const char* className = duk_get_string(ctx, 0);
            if (className) {
                if (node->hasClass(className)) {
                    node->addClass(className);
                } else {
                    node->removeClass(className);
                }
                node->sync();
            }
        }

        return 0;
    }

    duk_ret_t classList_replace(duk_context* ctx) {
        auto* node = getPtr<DOMNode>(ctx);

        // Toggle each class
        if (node) {
            const char* oldClassName = duk_get_string(ctx, 0);
            const char* newClassName = duk_get_string(ctx, 1);
            if (oldClassName && newClassName && node->hasClass(oldClassName)) {
                node->removeClass(oldClassName);
                node->addClass(newClassName);
                node->sync();
            }
        }

        return 0;
    }
}
