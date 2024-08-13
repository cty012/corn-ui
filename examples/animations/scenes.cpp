#include "scenes.h"

MainMenuScene::MainMenuScene() {
    // UI
    this->ui_.init("resources/ui/views/main_menu.xml", this->getUIManager());
}

MainMenuScene::~MainMenuScene() = default;
