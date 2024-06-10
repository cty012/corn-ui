#include <fstream>
#include <sstream>
#include <cornui/js/runtime.h>
#include "class_list.h"
#include "console.h"
#include "document.h"
#include "dom_node.h"
#include "runtime_impl.h"

namespace cornui {
    JSRuntimeImpl::JSRuntimeImpl(DOM& dom) {
        this->ctx_ = duk_create_heap_default();

        // Initialize the "console" object
        create_console(this->ctx_);

        // Initialize the classList prototype
        create_classList_prototype(this->ctx_);

        // Initialize the DOMNode prototype
        create_domNode_prototype(this->ctx_);

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
        if (!this->impl_) return;

        // Read the file
        std::ifstream filestr(file);
        std::stringstream codeStream;
        codeStream << filestr.rdbuf();
        std::string code = codeStream.str();

        // Execute the file content
        duk_peval_string_noresult(this->impl_->ctx_, code.c_str());
    }

    JSRuntimeImpl* JSRuntime::getImpl() noexcept {
        return this->impl_;
    }

    const JSRuntimeImpl* JSRuntime::getImpl() const noexcept {
        return this->impl_;
    }
}
