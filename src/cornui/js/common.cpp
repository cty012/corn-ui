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
