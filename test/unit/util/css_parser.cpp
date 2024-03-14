#include <gtest/gtest.h>
#include <cornui/css/cssom.h>
#include <cornui/util/css_parser.h>
#include <cornui/util/exception.h>
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
        str = "This-is_a-colon: this is a semicolon;\n-There : can: be :multiple : semi:colons;";
        ans = {
                { "This-is_a-colon", "this is a semicolon" },
                { "-There", "can: be :multiple : semi:colons" }
        };
        UnorderedMapsEqual(parseDeclFromString(str), ans);

        // Missing delimiter
        str = "hello world;";
        EXPECT_THROW(parseDeclFromString(str), CSSDeclSyntaxError);
        str = "hello: world \n ";
        EXPECT_THROW(parseDeclFromString(str), CSSDeclSyntaxError);

        // Empty identifier
        str = "\n : hello world;";
        EXPECT_THROW(parseDeclFromString(str), CSSDeclSyntaxError);

        // Invalid identifier
        str = "\nLast Name: Engine;";
        EXPECT_THROW(parseDeclFromString(str), CSSDeclSyntaxError);
        str = "\n1st-name: Corn;";
        EXPECT_THROW(parseDeclFromString(str), CSSDeclSyntaxError);
    }

    TEST(CSSParser, parse_css_from_stream) {
        std::istringstream stream;
        std::vector<CSSRule> ans;
        std::vector<CSSRule> result;

        // Empty string
        stream = std::istringstream("");
        ans = {};
        result = parseCSSFromStream(stream);
        EXPECT_EQ(result.size(), ans.size());
        for (size_t i = 0; i < ans.size(); i++) {
            EXPECT_EQ(result[i].selector.str, ans[i].selector.str);
            UnorderedMapsEqual(result[i].declarations, ans[i].declarations);
        }

        // Spaces only
        stream = std::istringstream("\v \r  \n\t   \r  ");
        ans = {};
        result = parseCSSFromStream(stream);
        EXPECT_EQ(result.size(), ans.size());
        for (size_t i = 0; i < ans.size(); i++) {
            EXPECT_EQ(result[i].selector.str, ans[i].selector.str);
            UnorderedMapsEqual(result[i].declarations, ans[i].declarations);
        }

        // One declaration
        stream = std::istringstream(" .example  {hello:\tworld  ; } ");
        ans = {
                {".example", {
                    { "hello", "world" }
                } }
        };
        result = parseCSSFromStream(stream);
        EXPECT_EQ(result.size(), ans.size());
        for (size_t i = 0; i < ans.size(); i++) {
            EXPECT_EQ(result[i].selector.str, ans[i].selector.str);
            UnorderedMapsEqual(result[i].declarations, ans[i].declarations);
        }

        // Multiple declarations
        stream = std::istringstream(
                "#id1   {\nhello: \tworld  ;\n\r\n good-bye :world; }#id2{lname:Engine;fname:Corn;}");
        ans = {
                {"#id1", {
                        { "hello", "world" },
                        { "good-bye", "world" }
                } },
                {"#id2", {
                        { "lname", "Engine" },
                        { "fname", "Corn" }
                } }
        };
        result = parseCSSFromStream(stream);
        EXPECT_EQ(result.size(), ans.size());
        for (size_t i = 0; i < ans.size(); i++) {
            EXPECT_EQ(result[i].selector.str, ans[i].selector.str);
            UnorderedMapsEqual(result[i].declarations, ans[i].declarations);
        }

        // Missing delimiter
        stream = std::istringstream(".example\n\thello:world;\n}");
        EXPECT_THROW(parseCSSFromStream(stream), CSSRuleSyntaxError);
        stream = std::istringstream(".example{hello:world;\n");
        EXPECT_THROW(parseCSSFromStream(stream), CSSRuleSyntaxError);
        stream = std::istringstream(".example\nhello:world;");
        EXPECT_THROW(parseCSSFromStream(stream), CSSRuleSyntaxError);

        // Empty identifier
        stream = std::istringstream("{\n\thello: world;\n}");
        EXPECT_THROW(parseCSSFromStream(stream), CSSRuleSyntaxError);

        // Invalid identifier
        // @todo
    }
}
