#include "common.h"
#include "context_data.h"
#include "js_request.h"

namespace cornui {
    void create_Request(JSContext* ctx) {
        // Create the RequestManager class
        auto* contextData = (ContextData*)JS_GetContextOpaque(ctx);
        JSClassID& classID = contextData->classIDs["RequestManager"];
        JS_NewClassID(&classID);
        JSClassDef classDef = {
                "RequestManager", nullptr, nullptr, nullptr, nullptr
        };
        JS_NewClass(JS_GetRuntime(ctx), classID, &classDef);

        // Create prototype
        JSValue proto = JS_NewObject(ctx);

        // Attach "get" function
        JS_SetPropertyStr(
                ctx, proto, "get",
                JS_NewCFunction(ctx, js_request_get, "get", 0));

        // Save prototype
        JS_SetClassProto(ctx, classID, proto);
    }

    JSValue js_request(JSContext* ctx, Request* request) {
        // Get the class ID
        JSClassID classID;
        if (!getClassID(ctx, &classID, "RequestManager")) {
            return JS_ThrowInternalError(ctx, "RequestManager class is not registered");
        }

        // Create the object
        JSValue obj = JS_NewObjectClass(ctx, (int)classID);
        if (JS_IsException(obj)) {
            JS_FreeValue(ctx, obj);
            return JS_ThrowInternalError(ctx, "Failed to create RequestManager object");
        }
        JS_SetOpaque(obj, request);
        return obj;
    }

    JSValue js_request_get(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv) {
        auto* request = getOpaque<Request>(ctx, this_val, "RequestManager");

        // Check the arguments
        if (argc != 2) {
            return JS_ThrowTypeError(ctx, "RequestManager.get() expects 2 arguments");
        }
        if (!JS_IsString(argv[0])) {
            return JS_ThrowTypeError(ctx, "RequestManager.get() expects the first argument to be a string");
        }

        // Get the identifier and data
        std::string identifier;
        getString(ctx, &identifier, argv[0]);
        nlohmann::json data = to_njson(ctx, argv[1]);

        // Get the response
        nlohmann::json response = request->get(identifier, std::move(data));

        // Return the response
        return from_njson(ctx, response);
    }
}
