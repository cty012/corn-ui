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
}
