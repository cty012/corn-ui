#pragma once

#include <filesystem>
#include <corn/ui/ui_manager.h>
#include <cornui/xml/dom.h>
#include <cornui/js/runtime.h>

namespace cornui {
    class UI {
    public:
        /**
         * @brief Constructor. The DOM tree MUST be loaded from a XML file.
         * @param file The path to the XML file.
         * @param uiManager
         * @throw std::invalid_argument If file cannot be opened, or the contents of the file cannot be parsed.
         */
        UI(std::filesystem::path file, corn::UIManager& uiManager);

        [[nodiscard]] const std::filesystem::path& getFile() const noexcept;
        [[nodiscard]] DOM& getDOM() noexcept;
        [[nodiscard]] const DOM& getDOM() const noexcept;
        [[nodiscard]] JSRuntime& getJSRuntime() noexcept;
        [[nodiscard]] const JSRuntime& getJSRuntime() const noexcept;

    private:
        std::filesystem::path file_;
        DOM dom_;
        JSRuntime jsRuntime_;
    };
};
