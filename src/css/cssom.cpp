#include <fstream>
#include <sstream>
#include <vector>
#include <corn/util/string_utils.h>
#include <cornui/css/cssom.h>
#include <cornui/util/css_parser.h>

namespace cornui {
    CSSOM::CSSOM() noexcept {}

    CSSOM::~CSSOM() {}

    void CSSOM::loadFromString(const std::string& contents) {
        std::istringstream stream(contents);
        std::vector<CSSRule> rules = parseCSSFromStream(stream);
    }

    void CSSOM::loadFromFile(const std::string& file) {
        std::ifstream stream(file);
        std::vector<CSSRule> rules = parseCSSFromStream(stream);
    }
}
