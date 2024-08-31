#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <duktape.h>

namespace cornui {
    void push_prototype(duk_context* ctx, const char* name);

    void push_vec_of_string(duk_context* ctx, const std::vector<std::string>& target);
    void push_umap_of_string_string(duk_context* ctx, const std::unordered_map<std::string, std::string>& target);
    void push_umap_of_string_int(duk_context* ctx, const std::unordered_map<std::string, int>& target);

    template<typename T>
    T* getPtr(duk_context* ctx, const std::string& ptrName = "__ptr") {
        // Push "this" and the pointer onto the stack
        duk_push_this(ctx);
        std::string hidden = "\xFF" + ptrName;
        duk_get_prop_string(ctx, -1, hidden.c_str());

        // Retrieve the pointer
        T* ptr = static_cast<T*>(duk_to_pointer(ctx, -1));

        // Pop "this" and the pointer
        duk_pop_2(ctx);

        return ptr;
    }
}
