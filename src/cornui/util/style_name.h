#pragma once

#include <string>

namespace cornui {
    enum class StyleValueType {
        BOOLEAN,            ///< Can be either `true` or `false`.
        ENUM,               ///< Can be one of a list of allowed strings.
        INTEGER,            ///< Can be any integer.
        NUMBER,             ///< Can be any number.
        EXPRESSION,         ///< Can be a mathematical expression.
        COLOR,              ///< Can be an RGB(A) color in `#xxxxxx` or `#xxxxxxxx` format.
        STRING,             ///< Can be any string.
        NONE,               ///< Invalid style.
    };

    bool isValidStyleName(const std::string& name) noexcept;

    StyleValueType getStyleValueType(const std::string& name) noexcept;
}
