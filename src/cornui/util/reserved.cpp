#include <algorithm>
#include <vector>
#include <cornui/util/reserved.h>

bool tagIsReserved(const std::string& tag) noexcept {
    static const std::vector<std::string> reservedTags = {
            "head", "body", "widget", "label", "image"
    };
    return std::find(reservedTags.begin(), reservedTags.end(), tag) != reservedTags.end();
}

bool attrIsReserved(const std::string& name) noexcept {
    return name == "class" || name == "style";
}

bool attrIsScript(const std::string& name) noexcept {
    static const std::vector<std::string> scripts = {
            "onload",
            "onkeydown", "onkeyup", "ontext",
            "onclick", "onhover", "onenter", "onexit", "onscroll",
            "onfocus", "onunfocus",
    };
    return std::find(scripts.begin(), scripts.end(), name) != scripts.end();
}
