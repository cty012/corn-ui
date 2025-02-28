#include <corn/core/scene.h>
#include <cornui/ui.h>

namespace cornui {
    UI::UI() : dom_(nullptr), animation_(nullptr), jsRuntime_(nullptr) {}

    UI::~UI() {
        delete this->dom_;
        delete this->jsRuntime_;
    }

    void UI::init(std::filesystem::path file, corn::UIManager& uiManager) {
        // Reset
        this->file_ = std::move(file);
        delete this->jsRuntime_;
        if (this->animation_) {
            this->animation_->getScene().removeSystem(this->animation_);
            this->animation_ = nullptr;
        }
        delete this->dom_;
        this->dom_ = new DOM(this);
        this->jsRuntime_ = new JSRuntime();

        // Add animation system
        this->animation_ = uiManager.getScene().addSystem<SAnimation>(this);

        // Load DOM from file
        std::vector<std::filesystem::path> toLoad;
        this->dom_->init(this->file_, toLoad);
        this->dom_->bind(uiManager);

        // Load JS from file
        this->jsRuntime_->bind(*this->dom_);
        for (const std::filesystem::path& jsFile : toLoad) {
            this->jsRuntime_->addFile(jsFile);
        }

        // Invoke the "onload" properties of all nodes
        for (DOMNode* node : this->dom_->getAllNodes()) {
            node->runScriptInAttr("onload");
        }
    }

    const std::filesystem::path& UI::getFile() const noexcept {
        return this->file_;
    }

    DOM* UI::getDOM() noexcept {
        return this->dom_;
    }

    const DOM* UI::getDOM() const noexcept {
        return this->dom_;
    }

    JSRuntime* UI::getJSRuntime() noexcept {
        return this->jsRuntime_;
    }

    const JSRuntime* UI::getJSRuntime() const noexcept {
        return this->jsRuntime_;
    }
}
