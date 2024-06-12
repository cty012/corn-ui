#include <corn/core.h>
#include <corn/event.h>
#include <corn/util.h>
#include <cornui/js/js_event_args.h>
#include "scenes.h"
#include "constants.h"

int main() {
    corn::Config config;
    config.title = "Clangy Bird";
    config.width = WIDTH;
    config.height = HEIGHT;
    config.antialiasing = 4;
    corn::FontManager::instance().load(
            "noto-sans-zh", "resources/fonts/noto-sans-zh/static/NotoSansSC-Regular.ttf");

    // Events
    corn::EventScope eventScope;
    eventScope.addListener(
            "corn::input::exit", [](const corn::EventArgs&) {
                corn::EventManager::instance().emit(corn::EventArgsExit());
            });

    eventScope.addListener(
            "js::scene::push",
            [](const corn::EventArgs& args) {
                std::string sceneType = dynamic_cast<const cornui::EventArgsJS&>(args).payload;
                corn::Scene* newScene;
                if (sceneType == "settings-scene") {
                    newScene = new SettingsScene();
                } else if (sceneType == "tutorial-scene") {
                    newScene = new TutorialScene();
                } else if (sceneType == "game-scene") {
                    newScene = new GameScene();
                } else {
                    return;
                }
                corn::EventManager::instance().emit(
                        corn::EventArgsScene(corn::SceneOperation::PUSH, newScene));
            });

    eventScope.addListener(
            "js::scene::pop",
            [](const corn::EventArgs&) {
                corn::EventManager::instance().emit(
                        corn::EventArgsScene(corn::SceneOperation::POP, nullptr));
            });

    eventScope.addListener(
            "js::exit",
            [](const corn::EventArgs&) {
                corn::EventManager::instance().emit(corn::EventArgsExit());
            });

    // Game
    corn::Game game(new MainMenuScene(), config);
    game.run();

    corn::FontManager::instance().unload("noto-sans-zh");
    return 0;
}
