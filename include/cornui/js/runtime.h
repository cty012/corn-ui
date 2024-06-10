#pragma once

#include <filesystem>

namespace cornui {
    class DOM;
    class JSRuntimeImpl;
    class UI;

    class JSRuntime {
    public:
        friend class UI;

        /**
         * @brief Execute JavaScript file.
         * @param file Path to the file.
         */
        void addFile(const std::filesystem::path& file);

        [[nodiscard]] JSRuntimeImpl* getImpl() noexcept;
        [[nodiscard]] const JSRuntimeImpl* getImpl() const noexcept;

    private:
        JSRuntime();
        ~JSRuntime();
        JSRuntime(const JSRuntime& dom) = delete;
        JSRuntime& operator=(const JSRuntime& jsRuntime) = delete;

        void bind(DOM& dom);

        JSRuntimeImpl* impl_;
    };
}
