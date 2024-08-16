#pragma once

#include <filesystem>

namespace cornui {
    class DOM;
    class JSEngineImpl;
    class UI;

    class JSEngine {
    public:
        friend class UI;

        /**
         * @brief Execute JavaScript file.
         * @param file Path to the file.
         */
        void addFile(const std::filesystem::path& file);

        [[nodiscard]] JSEngineImpl* getImpl() noexcept;
        [[nodiscard]] const JSEngineImpl* getImpl() const noexcept;

    private:
        JSEngine();
        ~JSEngine();
        JSEngine(const JSEngine& dom) = delete;
        JSEngine& operator=(const JSEngine& jsRuntime) = delete;

        void bind(DOM& dom);

        JSEngineImpl* impl_;
    };
}
