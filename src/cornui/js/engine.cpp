#include <fstream>
#include <sstream>
#include <corn/core/scene.h>
#include <corn/event/event_manager.h>
#include <corn/ui/ui_manager.h>
#include <cornui/js/engine.h>
#include <cornui/xml/dom.h>
#include "class_list.h"
#include "common.h"
#include "console.h"
#include "context_data.h"
#include "document.h"
#include "dom_node.h"
#include "event.h"
#include "engine_impl.h"
#include "js_request.h"

namespace cornui {
    JSEngineImpl::JSEngineImpl(DOM& dom, Request& request) {
        this->runtime_ = JS_NewRuntime();
        this->ctx_ = JS_NewContext(this->runtime_);
        JS_SetContextOpaque(this->ctx_, new ContextData(&dom));

        // Get the global object
        JSValue global = JS_GetGlobalObject(this->ctx_);

        // Initialize the "console" object
        create_console(this->ctx_);

        // Initialize the "ClassList" class
        create_ClassList(this->ctx_);

        // Initialize the "DOMNode" class
        create_DOMNode(this->ctx_);

        // Initialize the "document" object
        create_document(this->ctx_);

        // Initialize the "EventManager" class
        create_EventManager(this->ctx_);

        // Initialize the "Event" and "GlobalEvent" objects
        JSValue event = js_event(this->ctx_, dom.getUIManager()->getScene().getEventManager());
        JSValue globalEvent = js_event(this->ctx_, corn::EventManager::instance());
        JS_SetPropertyStr(this->ctx_, global, "Event", event);
        JS_SetPropertyStr(this->ctx_, global, "GlobalEvent", globalEvent);

        // Initialize the "request" object
        create_Request(this->ctx_);
        JSValue req = js_request(this->ctx_, &request);
        JS_SetPropertyStr(this->ctx_, global, "Request", req);

        // Free the global object
        JS_FreeValue(this->ctx_, global);
    }

    JSEngineImpl::~JSEngineImpl() {
        delete (ContextData*)JS_GetContextOpaque(this->ctx_);
        JS_RunGC(this->runtime_);
        JS_FreeContext(this->ctx_);
        JS_FreeRuntime(this->runtime_);
    }

    JSEngine::JSEngine() : impl_(nullptr) {}

    JSEngine::~JSEngine() {
        delete impl_;
    }

    void JSEngine::bind(DOM& dom, Request& request) {
        this->impl_ = new JSEngineImpl(dom, request);
    }

    void JSEngine::addFile(const std::filesystem::path& file) {
        if (!this->impl_) return;

        // Read the file
        std::ifstream filestr(file);
        std::stringstream codeStream;
        codeStream << filestr.rdbuf();
        std::string code = codeStream.str();

        // Compile and run the file content
        JSValue result = JS_Eval(this->impl_->ctx_, code.c_str(), code.size(), file.c_str(), JS_EVAL_TYPE_GLOBAL);
        if (JS_IsException(result)) {
            JSValue exception = JS_GetException(this->impl_->ctx_);
            std::string exceptionStr;
            getString(this->impl_->ctx_, &exceptionStr, exception);
            JS_FreeValue(this->impl_->ctx_, exception);
            fprintf(stderr, "Error executing JS file: '%s'\n%s\n", file.c_str(), exceptionStr.c_str());
        }

        // Free the result/error
        JS_FreeValue(this->impl_->ctx_, result);
    }

    JSEngineImpl* JSEngine::getImpl() noexcept {
        return this->impl_;
    }

    const JSEngineImpl* JSEngine::getImpl() const noexcept {
        return this->impl_;
    }
}
