#include <cornui/util/exception.h>

namespace cornui {
    CSSParseFailed::CSSParseFailed(const std::string& input, const std::string& msg) noexcept {
        this->msg_ = "Error parsing CSS from string:\n    Input string: " + input
                     + "\n    Error: " + msg + "\n";
    }

    const char* CSSParseFailed::what() const noexcept {
        return this->msg_.c_str();
    }

    CSSParseFailed::CSSParseFailed() noexcept : msg_() {}

    CSSRuleSyntaxError::CSSRuleSyntaxError(const std::string& input, const std::string& msg) noexcept {
        this->msg_ = "Error parsing CSS rule from string:\n    Input string: " + input
                     + "\n    Syntax error: " + msg + "\n";
    }

    const char* CSSRuleSyntaxError::what() const noexcept {
        return this->msg_.c_str();
    }

    CSSRuleSyntaxError::CSSRuleSyntaxError() noexcept : msg_() {}

    CSSSelectorSyntaxError::CSSSelectorSyntaxError(const std::string& input, const std::string& msg) noexcept {
        this->msg_ = "Error parsing CSS selector from string:\n    Input string: " + input
                     + "\n    Syntax error: " + msg + "\n";
    }

    const char* CSSSelectorSyntaxError::what() const noexcept {
        return this->msg_.c_str();
    }

    CSSSelectorSyntaxError::CSSSelectorSyntaxError() noexcept : msg_() {}

    CSSDeclSyntaxError::CSSDeclSyntaxError(const std::string& input, const std::string& msg) noexcept {
        this->msg_ = "Error parsing CSS declaration from string:\n    Input string: " + input
                     + "\n    Syntax error: " + msg + "\n";
    }

    const char* CSSDeclSyntaxError::what() const noexcept {
        return this->msg_.c_str();
    }

    CSSDeclSyntaxError::CSSDeclSyntaxError() noexcept : msg_() {}
}
