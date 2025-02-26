#include <fstream>
#include <sstream>
#include <corn/core/scene.h>
#include <corn/event/event_manager.h>
#include <corn/ui/ui_manager.h>
#include <cornui/js/runtime.h>
#include <cornui/xml/dom.h>
#include "class_list.h"
#include "console.h"
#include "document.h"
#include "dom_node.h"
#include "event.h"
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

        // Initialize the "Event" object
        create_event(this->ctx_, "Event", dom.getUIManager()->getScene().getEventManager(), this->eventScope_);

        // Initialize the "GlobalEvent" object
        create_event(this->ctx_, "GlobalEvent", corn::EventManager::instance(), this->eventScope_);
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
        const std::ifstream filestr(file);
        std::stringstream codeStream;
        codeStream << filestr.rdbuf();
        const std::string code = codeStream.str();

        // Compile and run the file content
        if (duk_pcompile_string(this->impl_->ctx_, 0, code.c_str()) != 0) {
#ifdef _WIN32
            fprintf(stderr, "Error compiling JS file: '%ls'\n%s\n", file.c_str(), duk_safe_to_string(this->impl_->ctx_, -1));
#else
            fprintf(stderr, "Error compiling JS file: '%s'\n%s\n", file.c_str(), duk_safe_to_string(this->impl_->ctx_, -1));
#endif
        } else {
            // Call the function
            duk_pcall(this->impl_->ctx_, 0);
        }

        // Pop the result/error
        duk_pop(this->impl_->ctx_);
    }

    JSRuntimeImpl* JSRuntime::getImpl() noexcept {
        return this->impl_;
    }

    const JSRuntimeImpl* JSRuntime::getImpl() const noexcept {
        return this->impl_;
    }
}
