#include <corn/media/font.h>
#include <gtest/gtest.h>

namespace cornui::test {
    class Environment : public ::testing::Environment {
    public:
        // This method will be called once before all tests are run
        void SetUp() override {
            corn::FontManager::instance().load(
                    "noto-sans", "resources/fonts/noto-sans/NotoSans-Regular.ttf");
        }

        // This method will be called once after all tests have finished
        void TearDown() override {
            corn::FontManager::instance().unload("noto-sans");
        }
    };

    ::testing::Environment* const env = ::testing::AddGlobalTestEnvironment(new Environment());
}

// Entry point for running the test cases
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
