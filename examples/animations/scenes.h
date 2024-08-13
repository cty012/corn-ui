#pragma once

#include <corn/core.h>
#include <corn/ecs.h>
#include <corn/event.h>
#include <cornui/ui.h>

/// The menu scene
class MainMenuScene : public corn::Scene {
public:
    MainMenuScene();
    ~MainMenuScene() override;

private:
    cornui::UI ui_;
    corn::EventScope eventScope_;
};
