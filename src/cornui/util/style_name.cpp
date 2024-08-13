#include <unordered_map>
#include <vector>
#include "style_name.h"

namespace cornui {
    bool isValidStyleName(const std::string& name) noexcept {
        static const std::vector<std::string> validStyleNames = {
                "active",
                "x", "y", "w", "h",
                "z-order",
                "keyboard-interactable",
                "mouse-interactable",
                "overflow",
                "background",
                "opacity",
                "font-family",
                "font-size",
                "font-color",
                "font-variant",
        };

        return std::find(validStyleNames.begin(), validStyleNames.end(), name) != validStyleNames.end();
    }

    StyleValueType getStyleValueType(const std::string& name) noexcept {
        static const std::unordered_map<std::string, StyleValueType> styleValueTypes = {
                { "active",                   StyleValueType::BOOLEAN },
                { "x",                        StyleValueType::EXPRESSION },
                { "y",                        StyleValueType::EXPRESSION },
                { "w",                        StyleValueType::EXPRESSION },
                { "h",                        StyleValueType::EXPRESSION },
                { "z-order",                  StyleValueType::INTEGER },
                { "keyboard-interactable",    StyleValueType::BOOLEAN },
                { "mouse-interactable",       StyleValueType::BOOLEAN },
                { "overflow",                 StyleValueType::ENUM },
                { "background",               StyleValueType::COLOR },
                { "opacity",                  StyleValueType::INTEGER },
                { "font-family",              StyleValueType::STRING },
                { "font-size",                StyleValueType::INTEGER },
                { "font-color",               StyleValueType::COLOR },
                { "font-variant",             StyleValueType::ENUM }
        };

        return styleValueTypes.contains(name) ? styleValueTypes.at(name) : StyleValueType::NONE;
    }
}
