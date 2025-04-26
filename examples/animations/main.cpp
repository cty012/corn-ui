#include <corn/core.h>
#include <corn/event.h>
#include <corn/util.h>
#include <cornui/js/js_event_args.h>
#include "scenes.h"

int main() {
    corn::Config config;
    config.title = "Animations";
    config.width = 1280;
    config.height = 720;
    config.antialiasing = 4;
    corn::FontManager::instance().loadFromPath("noto-sans", "resources/fonts/noto-sans/NotoSans-Regular.ttf");
    corn::FontManager::instance().setDefault("noto-sans");

    // Events
    corn::EventScope eventScope;
    eventScope.addListener(
            "corn::input::exit", [](const corn::EventArgs&) {
                corn::EventManager::instance().emit(corn::EventArgsExit());
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

    corn::FontManager::instance().unloadAll();
    return 0;
}
