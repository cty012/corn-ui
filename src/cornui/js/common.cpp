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

    void push_njson(duk_context* ctx, const nlohmann::json& target) {
        if (target.is_null()) {
            duk_push_null(ctx);
        } else if (target.is_boolean()) {
            duk_push_boolean(ctx, target.get<bool>());
        } else if (target.is_number()) {
            if (target.is_number_integer()) {
                duk_push_int(ctx, target.get<int>());
            } else {
                duk_push_number(ctx, target.get<float>());
            }
        } else if (target.is_string()) {
            duk_push_string(ctx, target.get<std::string>().c_str());
        } else if (target.is_array()) {
            const duk_idx_t arrIdx = duk_push_array(ctx);
            for (size_t i = 0; i < target.size(); i++) {
                push_njson(ctx, target[i]);
                duk_put_prop_index(ctx, arrIdx, static_cast<duk_uarridx_t>(i));
            }
        } else if (target.is_object()) {
            const duk_idx_t objIdx = duk_push_object(ctx);
            for (auto& [key, value] : target.items()) {
                push_njson(ctx, value);
                duk_put_prop_string(ctx, objIdx, key.c_str());
            }
        } else {
            duk_push_undefined(ctx);
        }
    }

    nlohmann::json get_njson(duk_context* ctx, duk_idx_t idx) {
        if (duk_is_boolean(ctx, idx)) {
            return duk_to_boolean(ctx, idx) != 0;
        } else if (duk_is_number(ctx, idx)) {
            return duk_to_number(ctx, idx);
        } else if (duk_is_string(ctx, idx)) {
            return duk_to_string(ctx, idx);
        } else if (duk_is_array(ctx, idx)) {
            std::vector<nlohmann::json> arr;
            duk_enum(ctx, idx, DUK_ENUM_ARRAY_INDICES_ONLY);
            while (duk_next(ctx, -1, 1)) {
                arr.push_back(get_njson(ctx, -1));
                duk_pop(ctx);
            }
            duk_pop(ctx);
            return arr;
        } else if (duk_is_object(ctx, idx)) {
            std::unordered_map<std::string, nlohmann::json> obj;
            duk_enum(ctx, idx, DUK_ENUM_OWN_PROPERTIES_ONLY);
            while (duk_next(ctx, -1, 1)) {
                obj[duk_to_string(ctx, -2)] = get_njson(ctx, -1);
                duk_pop_2(ctx);
            }
            duk_pop(ctx);
            return obj;
        } else {
            return nlohmann::json::value_t::null;
        }
    }

}
