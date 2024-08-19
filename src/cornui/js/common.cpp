#include <quickjs/quickjs.h>
#include "common.h"
#include "context_data.h"

namespace cornui {
    JSValue from_njson(JSContext* ctx, const nlohmann::json& target) {
        // Create a new object
        JSValue object;

        // Categorize the JSON object
        if (target.is_object()) {
            object = JS_NewObject(ctx);
            // Loop through the JSON object
            for (const auto& [key, val] : target.items()) {
                // Store the key-val pair in the object
                JS_SetPropertyStr(ctx, object, key.c_str(), from_njson(ctx, val));
            }
        } else if (target.is_array()) {
            object = JS_NewArray(ctx);
            // Loop through the JSON array
            for (size_t i = 0; i < target.size(); i++) {
                // Store the value in the object
                JS_SetPropertyUint32(ctx, object, i, from_njson(ctx, target[i]));
            }
        } else if (target.is_string()) {
            // Store the string in the object
            object = JS_NewString(ctx, target.get<std::string>().c_str());
        } else if (target.is_number_integer()) {
            // Store the integer in the object
            object = JS_NewInt32(ctx, target.get<int>());
        } else if (target.is_number_float()) {
            // Store the float in the object
            object = JS_NewFloat64(ctx, target.get<double>());
        } else if (target.is_boolean()) {
            // Store the boolean in the object
            object = JS_NewBool(ctx, target.get<bool>());
        } else if (target.is_null()) {
            // Store the null in the object
            object = JS_NULL;
        } else {
            // Store the undefined in the object
            object = JS_UNDEFINED;
        }

        return object;
    }

    nlohmann::json to_njson(JSContext* ctx, JSValueConst value) {
        nlohmann::json result;

        if (JS_IsNull(value)) {
            result = nullptr;
        } else if (JS_IsBool(value)) {
            result = JS_ToBool(ctx, value) > 0;
        } else if (JS_IsNumber(value)) {
            int32_t intVal;
            if (JS_ToInt32(ctx, &intVal, value) == 0) {
                // If the conversion to int32 is successful, it's an integer
                result = intVal;
            } else {
                // Otherwise, treat it as a double
                double doubleVal;
                JS_ToFloat64(ctx, &doubleVal, value);
                result = doubleVal;
            }
        } else if (JS_IsString(value)) {
            std::string str;
            getString(ctx, &str, value);
            result = str;
        } else if (JS_IsArray(ctx, value)) {
            uint32_t length;
            JSValue length_val = JS_GetPropertyStr(ctx, value, "length");
            JS_ToUint32(ctx, &length, length_val);
            JS_FreeValue(ctx, length_val);

            result = nlohmann::json::array();
            for (uint32_t i = 0; i < length; ++i) {
                JSValue elem = JS_GetPropertyUint32(ctx, value, i);
                result.push_back(to_njson(ctx, elem));
                JS_FreeValue(ctx, elem);
            }
        } else if (JS_IsObject(value)) {
            result = nlohmann::json::object();
            JSPropertyEnum *tab;
            uint32_t len;
            if (JS_GetOwnPropertyNames(ctx, &tab, &len, value, JS_GPN_STRING_MASK | JS_GPN_ENUM_ONLY) == 0) {
                for (uint32_t i = 0; i < len; ++i) {
                    JSValue propValue = JS_GetProperty(ctx, value, tab[i].atom);
                    const char *key = JS_AtomToCString(ctx, tab[i].atom);
                    result[key] = to_njson(ctx, propValue);
                    JS_FreeCString(ctx, key);
                    JS_FreeValue(ctx, propValue);
                    JS_FreeAtom(ctx, tab[i].atom);
                }
                js_free(ctx, tab);
            }
        }

        return result;
    }

    bool getString(JSContext* ctx, std::string* target, JSValueConst value, bool strict) {
        // Check if the value is a string
        if (strict && !JS_IsString(value)) {
            return false;
        }

        // Get the C-string
        const char* str = JS_ToCString(ctx, value);
        if (!str) {
            return false;
        }

        // Copy the string
        *target = str;

        // Free the string
        JS_FreeCString(ctx, str);

        return true;
    }

    bool getClassID(JSContext* ctx, JSClassID* target, const std::string& className) {
        const auto* contextData = (const ContextData*)JS_GetContextOpaque(ctx);
        if (!contextData->classIDs.contains(className)) {
            return false;
        }

        *target = contextData->classIDs.at(className);
        return true;
    }
}
