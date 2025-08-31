#include "LoadLevelPopup.hpp"

LoadLevelPopup* LoadLevelPopup::create(unsigned int levelID) {
    auto ret = new LoadLevelPopup;
    if (ret->initAnchored(360.f, 100.f, levelID)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}

bool LoadLevelPopup::setup(unsigned int levelID) {
    setTitle("Socials Integration");
    setID("LoadLevelPopup"_spr);

    m_id = levelID;
    m_allowClose = false;

    auto glm = GameLevelManager::get();
    // if (glm->m_levelManagerDelegate) return false;

    m_message = cocos2d::CCLabelBMFont::create(fmt::format("Loading level {}...", levelID).c_str(), "bigFont.fnt");
    m_message->setScale(.7f);
    m_message->setID("content");
    m_mainLayer->addChildAtPosition(m_message, geode::Anchor::Center);

    m_closeBtn->setVisible(false);

    glm->m_levelManagerDelegate = this;
    glm->getOnlineLevels(GJSearchObject::create(SearchType::Search, fmt::to_string(levelID)));

    runAction(
        cocos2d::CCSequence::createWithTwoActions(
            cocos2d::CCDelayTime::create(5.f),
            geode::cocos::CallFuncExt::create([this]() {
                auto glm = GameLevelManager::get();
                glm->m_levelManagerDelegate = nullptr;
                m_message->setString("Something went wrong! (timeout)");
                m_message->setScale(.575f);
                m_allowClose = true;
                m_closeBtn->setVisible(true);
            })
        )
    );
    
    return true;
}

void LoadLevelPopup::loadLevelsFinished(cocos2d::CCArray* levels, char const* p1, int p2) {
    // donezo funzo
    geode::log::info("donezo funzo with {}", m_id);
    auto glm = GameLevelManager::get();
    glm->m_levelManagerDelegate = nullptr;

    if (levels->count() == 0) {
        geode::log::warn("zero levels found");
        geode::Notification::create(fmt::format("No level found with ID {}!", m_id), geode::NotificationIcon::Error)->show();
        m_allowClose = true;
        Popup::onClose(nullptr);
        return;
    }

    auto scene = cocos2d::CCTransitionFade::create(.5f, LevelInfoLayer::scene(static_cast<GJGameLevel*>(levels->objectAtIndex(0)), false));
    cocos2d::CCDirector::get()->pushScene(scene);
    m_allowClose = true;
    Popup::onClose(nullptr);
}

void LoadLevelPopup::loadLevelsFinished(cocos2d::CCArray* levels, char const* p1) {
    loadLevelsFinished(levels, p1, 0);
}

void LoadLevelPopup::loadLevelsFailed(char const* p0, int p1) {
    geode::log::warn("loadLevelsFailed");
    auto glm = GameLevelManager::get();
    glm->m_levelManagerDelegate = nullptr;
    geode::Notification::create(fmt::format("No level found with ID {}!", m_id), geode::NotificationIcon::Error)->show();
    m_allowClose = true;
    Popup::onClose(nullptr);
}

void LoadLevelPopup::loadLevelsFailed(char const* p0) {
    loadLevelsFailed(p0, 0);
}

void LoadLevelPopup::onClose(cocos2d::CCObject* sender) {
    if (!m_allowClose) return;
    
    Popup::onClose(sender);
}
