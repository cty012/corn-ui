#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <nlohmann/json.hpp>
#include <quickjs/quickjs.h>

namespace cornui {
//    JSValue from_vec_of_string(JSContext* ctx, const std::vector<std::string>& target);
//    JSValue from_umap_of_string_string(JSContext* ctx, const std::unordered_map<std::string, std::string>& target);
//    JSValue from_umap_of_string_int(JSContext* ctx, const std::unordered_map<std::string, int>& target);
//    JSValue from_umap_of_string_float(JSContext* ctx, const std::unordered_map<std::string, float>& target);
    JSValue from_njson(JSContext* ctx, const nlohmann::json& target);

    bool getString(JSContext* ctx, std::string* target, JSValueConst value, bool strict = false);

    bool getClassID(JSContext* ctx, JSClassID* target, const std::string& className);

    template <typename T>
    T* getOpaque(JSContext* ctx, JSValueConst this_val, const std::string& className) {
        JSClassID classID;
        if (!getClassID(ctx, &classID, className)) {
            return nullptr;
        }
        return static_cast<T*>(JS_GetOpaque(this_val, classID));
    }
}
