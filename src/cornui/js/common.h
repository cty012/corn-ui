#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <nlohmann/json.hpp>
#include <quickjs/quickjs.h>

namespace cornui {
    JSValue from_njson(JSContext* ctx, const nlohmann::json& target);

    nlohmann::json to_njson(JSContext* ctx, JSValueConst value);

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
