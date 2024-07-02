#include <gtest/gtest.h>
#include <cornui/css/cssom.h>
#include <cornui/util/css_parser.h>
#include <cornui/util/exception.h>
#include "utility.h"

namespace cornui::test::css_parser {
    std::string combinatorToString(const CSSSelectorCombinator& comb) {
        switch (comb) {
            case CSSSelectorCombinator::DESCENDANT:
                return "DESCENDANT";
            case CSSSelectorCombinator::CHILD:
                return "CHILD";
            case CSSSelectorCombinator::ADJACENT_SIBLING:
                return "ADJACENT_SIBLING";
            case CSSSelectorCombinator::GENERAL_SIBLING:
                return "GENERAL_SIBLING";
            case CSSSelectorCombinator::GROUP:
                return "GROUP";
            default:
                return "NONE";
        }
    }

    bool TestSelectorEqual(const CSSSelector& selector1, const CSSSelector& selector2) {
        EXPECT_EQ_RETURN(selector1.groups.size(), selector2.groups.size(), false);
        for (size_t i = 0; i < selector1.groups.size(); i++) {
            const CSSSelectorGroup& group1 = selector1.groups[i];
            const CSSSelectorGroup& group2 = selector2.groups[i];
            // Check that the CSS selector groups have same size
            EXPECT_EQ_RETURN(group1.basicSelectors.size(), group2.basicSelectors.size(), false);
            // Check that each basic selectors are the same
            for (size_t j = 0; j < group1.basicSelectors.size(); j++) {
                VectorsEqual(group1.basicSelectors[j].parts, group2.basicSelectors[j].parts);
            }
            // Check that each combinators are the same
            MappedVectorsEqual(group1.combinators, group2.combinators, std::function(combinatorToString));
        }
        return true;
    }

    bool TestCSSEqual(const std::vector<CSSRule>& css1, const std::vector<CSSRule>& css2) {
        EXPECT_EQ_RETURN(css1.size(), css2.size(), false);
        for (size_t i = 0; i < css1.size(); i++) {
            if (TestSelectorEqual(css1[i].selector, css2[i].selector)) return false;
            UnorderedMapsEqual(css1[i].declarations, css2[i].declarations);
        }
        return true;
    }

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
        TestCSSEqual(result, ans);

        // Spaces only
        stream = std::istringstream("\v \r  \n\t   \r  ");
        ans = {};
        result = parseCSSFromStream(stream);
        TestCSSEqual(result, ans);

        // One declaration
        stream = std::istringstream(" .example  {hello:\tworld  ; } ");
        ans = {
                {{{{{{{ ".example" }}}, {}}}}, {
                    { "hello", "world" }
                }}
        };
        result = parseCSSFromStream(stream);
        TestCSSEqual(result, ans);

        // Multiple declarations
        stream = std::istringstream(
                "#id1   {\nhello: \tworld  ;\n\r\n good-bye :world; }#id2, #id3 .cls1{lname:Engine;fname:Corn;}");
        ans = {
                {{{{{{{ "#id1" }}}, {}}}}, {
                        { "hello", "world" },
                        { "good-bye", "world" }
                }},
                {{{
                    {{{{ "#id2" }}}, {}},
                    {{{{ "#id3" }}, {{ ".cls1" }}}, {CSSSelectorCombinator::DESCENDANT}}
                }}, {
                        { "lname", "Engine" },
                        { "fname", "Corn" }
                }}
        };
        result = parseCSSFromStream(stream);
        TestCSSEqual(result, ans);

        // Missing delimiter
        stream = std::istringstream(".example\n\thello:world;\n}");
        EXPECT_THROW(parseCSSFromStream(stream), CSSRuleSyntaxError);
        stream = std::istringstream(".example{hello:world;\n");
        EXPECT_THROW(parseCSSFromStream(stream), CSSRuleSyntaxError);
        stream = std::istringstream(".example\nhello:world;");
        EXPECT_THROW(parseCSSFromStream(stream), CSSRuleSyntaxError);

        // Empty identifier
        stream = std::istringstream("{\n\thello: world;\n}");
        EXPECT_THROW(parseCSSFromStream(stream), CSSSelectorSyntaxError);

        // Invalid identifier
        // @todo
    }
}
