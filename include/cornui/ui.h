#pragma once

#include <filesystem>
#include <corn/ui/ui_manager.h>
#include <cornui/css/animation.h>
#include <cornui/xml/dom.h>
#include <cornui/js/engine.h>
#include <cornui/js/request.h>

namespace cornui {
    class UI {
    public:
        UI();
        ~UI();
        UI(const UI& other) = delete;
        UI& operator=(const UI& other) = delete;

        /**
         * @brief Initialize the UI.
         * @param file The path to the XML file.
         * @param uiManager The UI manager to bind to.
         * @throw std::invalid_argument If file cannot be opened, or the contents of the file cannot be parsed.
         */
        void init(std::filesystem::path file, corn::UIManager& uiManager);

        [[nodiscard]] const std::filesystem::path& getFile() const noexcept;
        [[nodiscard]] DOM* getDOM() noexcept;
        [[nodiscard]] const DOM* getDOM() const noexcept;
        [[nodiscard]] Request& getRequest() noexcept;
        [[nodiscard]] const Request& getRequest() const noexcept;
        [[nodiscard]] JSEngine* getJSEngine() noexcept;
        [[nodiscard]] const JSEngine* getJSEngine() const noexcept;

    private:
        std::filesystem::path file_;
        DOM* dom_;
        SAnimation* animation_;
        Request request_;
        JSEngine* jsEngine_;
    };
};
