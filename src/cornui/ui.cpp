#include <cornui/ui.h>

namespace cornui {
    UI::UI(std::filesystem::path file, corn::UIManager& uiManager) : file_(std::move(file)), dom_() {
        std::vector<std::filesystem::path> toLoad;
        this->dom_.init(this->file_, toLoad);
        this->dom_.bind(uiManager);

        this->jsRuntime_.bind(this->dom_);
        for (const std::filesystem::path& jsFile : toLoad) {
            this->jsRuntime_.addFile(jsFile);
        }
    }

    const std::filesystem::path& UI::getFile() const noexcept {
        return this->file_;
    }

    DOM& UI::getDOM() noexcept {
        return this->dom_;
    }

    const DOM& UI::getDOM() const noexcept {
        return this->dom_;
    }

    JSRuntime& UI::getJSRuntime() noexcept {
        return this->jsRuntime_;
    }

    const JSRuntime& UI::getJSRuntime() const noexcept {
        return this->jsRuntime_;
    }
}
