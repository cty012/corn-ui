#include <cornui/js/js_event_args.h>
#include "common.h"
#include "context_data.h"
#include "event.h"

namespace cornui {
    void create_EventManager(JSContext* ctx) {
        // Create the EventManager class
        auto* contextData = (ContextData*)JS_GetContextOpaque(ctx);
        JSClassID& classID = contextData->classIDs["EventManager"];
        JS_NewClassID(&classID);
        JSClassDef classDef = {
                "EventManager", nullptr, nullptr, nullptr, nullptr
        };
        JS_NewClass(JS_GetRuntime(ctx), classID, &classDef);

        // Create prototype
        JSValue proto = JS_NewObject(ctx);

        // Attach "emit" function
        JS_SetPropertyStr(
                ctx, proto, "emit",
                JS_NewCFunction(ctx, js_event_emit, "emit", 1));

        // Save prototype
        JS_SetClassProto(ctx, classID, proto);
    }

    JSValue js_event(JSContext* ctx, corn::EventManager& eventManager) {
        // Get the class ID
        JSClassID classID;
        if (!getClassID(ctx, &classID, "EventManager")) {
            return JS_ThrowInternalError(ctx, "EventManager class is not registered");
        }

        // Create the object
        JSValue obj = JS_NewObjectClass(ctx, (int)classID);
        if (JS_IsException(obj)) {
            JS_FreeValue(ctx, obj);
            return JS_ThrowInternalError(ctx, "Failed to create DOMNode object");
        }
        JS_SetOpaque(obj, &eventManager);
        return obj;
    }

    JSValue js_event_emit(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv) {
        auto* eventManager = getOpaque<corn::EventManager>(ctx, this_val, "EventManager");

        // Check the arguments
        if (argc != 1 && argc != 2) {
            return JS_ThrowTypeError(ctx, "EventManager.emit() expects 1 or 2 arguments");
        }
        if (!JS_IsString(argv[0])) {
            return JS_ThrowTypeError(ctx, "EventManager.emit() expects the first argument to be a string");
        }

        // Get the event type and payload
        std::string type, payload;
        getString(ctx, &type, argv[0]);
        if (argc == 2) {
            if (!getString(ctx, &payload, argv[1])) {
                return JS_ThrowTypeError(ctx, "EventManager.emit() expects the second argument to be convertible to string");
            }
        }

        // Emit the event
        eventManager->emit(EventArgsJS("js::" + type, payload));

        return JS_UNDEFINED;
    }
}
