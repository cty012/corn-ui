#include "common.h"

namespace cornui {
    void push_prototype(duk_context* ctx, const char* name) {
        // The target object
        duk_idx_t objIdx = duk_push_object(ctx);

        // Push the global stash and the prop string
        duk_push_global_stash(ctx);
        duk_get_prop_string(ctx, -1, name);

        // Set the target object's prototype
        duk_set_prototype(ctx, objIdx);

        // Pop the global stash
        duk_pop(ctx);
    }

    void push_vec_of_string(duk_context* ctx, const std::vector<std::string>& target) {
        // Create a new array
        duk_idx_t arrayIdx = duk_push_array(ctx);

        // Loop through the vector of strings
        for (size_t i = 0; i < target.size(); i++) {
            // Add the item to the array
            duk_push_string(ctx, target[i].c_str());
            duk_put_prop_index(ctx, arrayIdx, i);
        }
    }

    void push_umap_of_string_string(duk_context* ctx, const std::unordered_map<std::string, std::string>& target) {
        duk_idx_t objectIdx = duk_push_object(ctx);

        // Loop through the umap of string -> string
        for (const auto& [key, val] : target) {
            // Store the key-val pair in the object
            duk_push_string(ctx, val.c_str());
            duk_put_prop_string(ctx, objectIdx, key.c_str());
        }
    }
}
