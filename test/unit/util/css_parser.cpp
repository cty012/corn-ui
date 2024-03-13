#include <gtest/gtest.h>
#include <cornui/util/css_parser.h>
#include "utility.h"

namespace cornui::test::css_parser {
    TEST(CSSParser, parse_decl_from_string) {
        std::string str;
        std::unordered_map<std::string, std::string> ans;

        // Empty string
        str = "";
        ans = {};
        UnorderedMapsEqual(parseDeclFromString(str), ans);

        // Spaces only
        str = "\v \r  \n\t   \r  ";
        ans = {};
        UnorderedMapsEqual(parseDeclFromString(str), ans);

        // One declaration
        str = "  hello:\tworld  ;  ";
        ans = { { "hello", "world" } };
        UnorderedMapsEqual(parseDeclFromString(str), ans);

        // Multiple declarations
        str = "hello: \tworld  ;\n\r\n good-bye :world; ";
        ans = { { "hello", "world" }, { "good-bye", "world" } };
        UnorderedMapsEqual(parseDeclFromString(str), ans);

        // Declarations that may seem weird but is correct
        str = "This is a colon: this is a semicolon;\nThere : can: be :multiple : semi:colons;";
        ans = {
                { "This is a colon", "this is a semicolon" },
                { "There", "can: be :multiple : semi:colons" }
        };
        UnorderedMapsEqual(parseDeclFromString(str), ans);
    }
}
