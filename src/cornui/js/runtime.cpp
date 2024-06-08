#include <fstream>
#include <sstream>
#include <cornui/js/runtime.h>
#include "document.h"
#include "runtime_impl.h"

namespace cornui {
    JSRuntimeImpl::JSRuntimeImpl(DOM& dom) {
        this->ctx_ = duk_create_heap_default();

        // Initialize the "document" object
        create_document(this->ctx_, &dom);
    }

    JSRuntimeImpl::~JSRuntimeImpl() {
        duk_destroy_heap(this->ctx_);
    }

    JSRuntime::JSRuntime() : impl_(nullptr) {}

    JSRuntime::~JSRuntime() {
        delete impl_;
    }

    void JSRuntime::bind(cornui::DOM& dom) {
        this->impl_ = new JSRuntimeImpl(dom);
    }

    void JSRuntime::addFile(const std::filesystem::path& file) {
        // Read the file
        std::ifstream filestr(file);
        std::stringstream code;
        code << filestr.rdbuf();
        duk_peval_string(this->impl_->ctx_, code.str().c_str());
    }
}
