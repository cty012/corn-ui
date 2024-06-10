#pragma once

#include <corn/core.h>
#include <corn/ecs.h>
#include <corn/event.h>
#include <cornui/ui.h>
#include "constants.h"

/// The menu scene
class MainMenuScene : public corn::Scene {
public:
    MainMenuScene();
    ~MainMenuScene() override;

private:
    cornui::UI ui_;
    corn::EventManager::ListenerID langChangeEventID_;
};

/// The settings scene
class SettingsScene : public corn::Scene {
public:
    SettingsScene();
    ~SettingsScene() override;

private:
    cornui::UI ui_;
    corn::EventManager::ListenerID langChangeEventID_;
};

/// The tutorial scene
class TutorialScene : public corn::Scene {
public:
    TutorialScene();
    ~TutorialScene() override;

private:
    corn::EventManager::ListenerID langChangeEventID_;
};

/// The main game scene
class GameScene : public corn::Scene {
public:
    GameScene();
    ~GameScene() override;

    [[nodiscard]] bool isPaused() const;
    void togglePause();

private:
    bool paused_;
    corn::UIWidget* pauseMenu_;
    corn::Entity* bird_;
    corn::CMovement2D* birdMovement_;
    std::vector<corn::System*> addedSystems_;
    corn::EventManager::ListenerID keyboardEventID_;
    corn::EventManager::ListenerID mouseEventID_;
    corn::EventManager::ListenerID langChangeEventID_;

    void onKeyboardEvent(const corn::EventArgsKeyboard& args);
    void onMouseEvent(const corn::EventArgsMouseButton& args);
};
