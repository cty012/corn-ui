#include <fstream>
#include <sstream>
#include <vector>
#include <corn/util/string_utils.h>
#include <cornui/css/cssom.h>
#include <cornui/util/css_parser.h>

namespace cornui {
    CSSOM::CSSOM() noexcept = default;

    CSSOM::~CSSOM() = default;

    CSSOM& CSSOM::instance() noexcept {
        static CSSOM singleton;
        return singleton;
    }

    void CSSOM::loadFromStream(std::istream& input) {
        for (const CSSRule& rule : parseCSSFromStream(input)) {
            this->addRule(rule);
        }
    }

    void CSSOM::loadFromString(const std::string& contents) {
        std::istringstream input(contents);
        this->loadFromStream(input);
    }

    void CSSOM::loadFromFile(const std::filesystem::path& file) {
        std::ifstream input(file);
        this->loadFromStream(input);
    }

    void CSSOM::addRule(const CSSRule& rule) {
        // Currently no reduction is applied
        this->rules_.push_back(rule);
    }
}
