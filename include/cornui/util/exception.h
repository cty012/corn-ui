#pragma once

#include <stdexcept>
#include <string>

namespace cornui {
    /**
     * @class CSSParseFailed
     * @brief Base class of all errors related to parsing CSS.
     */
    class CSSParseFailed : public std::exception {
    public:
        CSSParseFailed(const std::string& input, const std::string& msg) noexcept;
        [[nodiscard]] const char* what() const noexcept override;

    protected:
        CSSParseFailed() noexcept;

    private:
        std::string msg_;
    };

    /**
     * @class CSSRuleSyntaxError
     * @brief Syntax error when parsing a CSS rule.
     */
    class CSSRuleSyntaxError : public std::exception {
    public:
        CSSRuleSyntaxError(const std::string& input, const std::string& msg) noexcept;
        [[nodiscard]] const char* what() const noexcept override;

    protected:
        CSSRuleSyntaxError() noexcept;

    private:
        std::string msg_;
    };

    /**
     * @class CSSSelectorSyntaxError
     * @brief Syntax error when parsing a CSS selector.
     */
    class CSSSelectorSyntaxError : public std::exception {
    public:
        CSSSelectorSyntaxError(const std::string& input, const std::string& msg) noexcept;
        [[nodiscard]] const char* what() const noexcept override;

    protected:
        CSSSelectorSyntaxError() noexcept;

    private:
        std::string msg_;
    };

    /**
     * @class CSSDeclSyntaxError
     * @brief Syntax error when parsing a CSS declaration.
     */
    class CSSDeclSyntaxError : public std::exception {
    public:
        CSSDeclSyntaxError(const std::string& input, const std::string& msg) noexcept;
        [[nodiscard]] const char* what() const noexcept override;

    protected:
        CSSDeclSyntaxError() noexcept;

    private:
        std::string msg_;
    };
}
