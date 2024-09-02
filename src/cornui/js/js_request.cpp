#include "common.h"
#include "js_request.h"

namespace cornui {
    void create_Request(duk_context* ctx, const std::string& name, Request* request) {
        // Push the global object and the request object onto the stack
        duk_push_global_object(ctx);
        const duk_idx_t requestIdx = duk_push_object(ctx);

        // Add the Request pointer as hidden property
        duk_push_pointer(ctx, request);
        duk_put_prop_string(ctx, requestIdx, DUK_HIDDEN_SYMBOL("__ptr"));

        // Add function "get" to the request object
        duk_push_c_function(ctx, request_get, 2);
        duk_put_prop_string(ctx, requestIdx, "get");

        // Add the request object to the global object
        duk_put_prop_string(ctx, -2, name.c_str());

        // Pop the global object
        duk_pop(ctx);
    }

    duk_ret_t request_get(duk_context* ctx) {
        auto* request = getPtr<Request>(ctx);

        // Check the arguments
        if (!duk_is_string(ctx, 0)) {
            return 0;
        }

        // Get the identifier and data
        const char* identifier = duk_to_string(ctx, 0);
        nlohmann::json data = get_njson(ctx, 1);

        // Get the response
        const nlohmann::json response = request->get(identifier, std::move(data));
        push_njson(ctx, response);

        return 0;
    }
}
