#pragma once

#include <string>
#include <duktape.h>
#include <nlohmann/json.hpp>

namespace cornui {
    void push_prototype(duk_context* ctx, const char* name);

    void push_njson(duk_context* ctx, const nlohmann::json& target);

    nlohmann::json get_njson(duk_context* ctx, duk_idx_t idx);

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
