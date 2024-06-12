#include <corn/ui.h>
#include <cornui/js/js_event_args.h>
#include "entities.h"
#include "event_args.h"
#include "scenes.h"
#include "systems.h"
#include "text_manager.h"

void updateText(cornui::UI& ui, const std::string& tag, const std::string& key) {
    cornui::DOMNode* node = ui.getDOM()->getNodeBySelector(tag);
    corn::RichText richText = TextManager::instance().getRichText(key);
    node->setText(richText.segments[0].str);
    node->setStyle("font-size", std::to_string(richText.segments[0].style.size) + "px");
    node->setStyle("font-color", richText.segments[0].style.color.hexStringAlpha());
}

MainMenuScene::MainMenuScene() {
    // UI
    this->ui_.init("resources/ui/views/main_menu.xml", this->getUIManager());

    this->eventScope_.addListener(
            "clangy-bird::langchange",
            [this](const corn::EventArgs&) {
                updateText(this->ui_, "#title", "main-menu-title");
                updateText(this->ui_, "#start", "main-menu-start");
                updateText(this->ui_, "#settings", "main-menu-settings");
                updateText(this->ui_, "#tutorial", "main-menu-tutorial");
                updateText(this->ui_, "#exit", "main-menu-exit");
            });

    // Initialize text
    corn::EventManager::instance().emit(EventArgsLanguageChange());
}

MainMenuScene::~MainMenuScene() = default;

SettingsScene::SettingsScene() {
    // UI
    this->ui_.init("resources/ui/views/settings.xml", this->getUIManager());

    // Language change button
    auto& lang = *(corn::UILabel*)this->getUIManager().getWidgetByName("lang");
    lang.getEventManager().addListener(
            "corn::ui::onclick",
            [](const corn::EventArgs& args) {
                if (dynamic_cast<const corn::EventArgsUIOnClick&>(args).mousebtnEvent.status != corn::ButtonEvent::UP) {
                    return;
                }
                std::string language = (std::string)TextManager::instance().getSettings("lang");
                TextManager::instance().changeSettings("lang", language == "eng" ? "chn" : "eng");
                TextManager::instance().saveSettings();
                corn::EventManager::instance().emit(EventArgsLanguageChange());
            });

    this->eventScope_.addListener(
            "clangy-bird::langchange",
            [this](const corn::EventArgs&) {
                std::string language = (std::string)TextManager::instance().getSettings("lang");
                updateText(this->ui_, "#title", "settings-title");
                updateText(this->ui_, "#lang-label", "settings-lang");
                updateText(this->ui_, "#lang", "settings-lang-" + language);
                updateText(this->ui_, "#save", "settings-save");
            });

    // Initialize text
    corn::EventManager::instance().emit(EventArgsLanguageChange());
}

SettingsScene::~SettingsScene() = default;

TutorialScene::TutorialScene() {
    // UI
    this->ui_.init("resources/ui/views/tutorial.xml", this->getUIManager());

    this->eventScope_.addListener(
            "clangy-bird::langchange",
            [this](const corn::EventArgs&) {
                updateText(this->ui_, "#title", "tutorial-title");
                updateText(this->ui_, "#text", "tutorial-text");
                updateText(this->ui_, "#back", "tutorial-back");
            });

    // Initialize text
    corn::EventManager::instance().emit(EventArgsLanguageChange());
}

TutorialScene::~TutorialScene() = default;

GameScene::GameScene() : paused_(false), addedSystems_() {
    // Camera
    createCamera(this->getEntityManager());

    // Entities
    this->bird_ = createBird(this->getEntityManager());
    this->birdMovement_ = this->bird_->getComponent<corn::CMovement2D>();
    createCeilAndFloor(this->getEntityManager());

    // Systems
    this->addedSystems_.push_back(this->addSystem<corn::SMovement2D>());
    this->addedSystems_.push_back(this->addSystem<corn::SGravity>());
    this->addedSystems_.push_back(this->addSystem<corn::SCollisionDetection>());
    this->addedSystems_.push_back(this->addSystem<WallManager>());
    this->addedSystems_.push_back(this->addSystem<BirdCollisionResolve>());

    // UI
    this->ui_.init("resources/ui/views/pause_menu.xml", this->getUIManager());

    // Event listeners
    this->eventScope_.addListener(
            this->getEventManager(),
            "js::toggle-pause",
            [this](const corn::EventArgs&) {
                this->togglePause();
            });

    this->eventScope_.addListener(
            this->getEventManager(),
            "corn::input::keyboard",
            [this](const corn::EventArgs &args) {
                this->onKeyboardEvent(dynamic_cast<const corn::EventArgsKeyboard&>(args));
            });

    this->eventScope_.addListener(
            this->getEventManager(),
            "corn::input::mousebtn",
            [this](const corn::EventArgs &args) {
                this->onMouseEvent(dynamic_cast<const corn::EventArgsMouseButton&>(args));
            });

    this->eventScope_.addListener(
            "clangy-bird::langchange",
            [this](const corn::EventArgs&) {
                updateText(this->ui_, "#title", "game-pause-title");
                updateText(this->ui_, "#continue", "game-pause-continue");
                updateText(this->ui_, "#exit-to-main-menu", "game-pause-exit-to-main-menu");
            });

    // Initialize text
    corn::EventManager::instance().emit(EventArgsLanguageChange());
}

GameScene::~GameScene() = default;

bool GameScene::isPaused() const {
    return this->paused_;
}

void GameScene::togglePause() {
    this->paused_ = !this->paused_;
    this->ui_.getDOM()->getNodeBySelector("#pause-screen")
            ->setStyle("active", this->paused_ ? "true" : "false");
    for (corn::System* system : this->addedSystems_) {
        system->setActive(!this->paused_);
    }
}

void GameScene::onKeyboardEvent(const corn::EventArgsKeyboard& args) {
    if (args.status != corn::ButtonEvent::DOWN) return;
    switch (args.key) {
        case corn::Key::W:
        case corn::Key::UP:
        case corn::Key::SPACE:
            if (!this->paused_) {
                this->birdMovement_->velocity.y = -700;
            }
            break;
        case corn::Key::ESC:
            this->togglePause();
        default:
            break;
    }
}

void GameScene::onMouseEvent(const corn::EventArgsMouseButton& args) {
    if (!this->paused_ && args.mouse == corn::Mouse::LEFT && args.status == corn::ButtonEvent::DOWN) {
        this->birdMovement_->velocity.y = -700;
    }
}
