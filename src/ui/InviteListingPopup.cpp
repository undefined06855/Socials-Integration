#include "InviteListingPopup.hpp"
#include "../InviteManager.hpp"
#include "FriendCell.hpp"

InviteListingPopup* InviteListingPopup::create() {
    auto ret = new InviteListingPopup;
    if (ret->initAnchored(184.f, 302.f, "GJ_square02.png")) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}

bool InviteListingPopup::setup() {
    auto [level, lobby] = getInvitePreviewInfo();

    m_closing = false;

    setTitle(fmt::format("Invite to play {}", level));
    setID("InviteListingPopup"_spr);

    auto winSize = cocos2d::CCDirector::get()->getWinSize();

    // where we're going, we don't need buttons
    m_buttonMenu->removeFromParent();

    // make the popup close when you click off of it
    auto fakeCloseMenu = cocos2d::CCMenu::create();

    auto fakeCloseSprite = cocos2d::CCNode::create();
    fakeCloseSprite->setContentSize(winSize);
    auto fakeCloseButton = geode::cocos::CCMenuItemExt::createSpriteExtra(
        fakeCloseSprite,
        [this](auto) {
            onClose(nullptr);
        }
    );
    fakeCloseButton->setZOrder(1);
    fakeCloseButton->setUserObject("undefined0.controllable/do-not-focus", cocos2d::CCBool::create(true));
    fakeCloseMenu->addChildAtPosition(fakeCloseButton, geode::Anchor::Center);

    // dont make the popup close if you click on it
    // obviously
    auto blockerSprite = cocos2d::CCNode::create();
    blockerSprite->setContentSize({ 215.f, winSize.height });
    auto closeButtonBlocker = geode::cocos::CCMenuItemExt::createSpriteExtra(
        blockerSprite,
        [](auto) {}
    );
    closeButtonBlocker->setAnchorPoint({ 1.f, .5f });
    closeButtonBlocker->setZOrder(-1);
    closeButtonBlocker->setUserObject("undefined0.controllable/do-not-focus", cocos2d::CCBool::create(true));
    fakeCloseMenu->addChildAtPosition(closeButtonBlocker, geode::Anchor::Right);

    // TODO: FIX TOUCH PRIO ????????????
    geode::Loader::get()->queueInMainThread([fakeCloseMenu = geode::Ref(fakeCloseMenu)]() {
        auto prio = cocos2d::CCTouchDispatcher::get()->findHandler(fakeCloseMenu)->getPriority();
        cocos2d::CCTouchDispatcher::get()->setPriority(prio + 2, fakeCloseMenu);
    });

    addChild(fakeCloseMenu); // not to m_mainLayer!

    if (!lobby.empty()) {
        auto subtitle = cocos2d::CCLabelBMFont::create(fmt::format("And join \"{}\"", lobby).c_str(), "goldFont.fnt");
        subtitle->setScale(.35f);
        subtitle->setID("subtitle");
        m_mainLayer->addChildAtPosition(subtitle, geode::Anchor::Top, { 0.f, -30.f });
    }
    auto scrollLayer = geode::ScrollLayer::create({ 170.f, 250.f });
    scrollLayer->ignoreAnchorPointForPosition(false); // ???

    scrollLayer->m_contentLayer->setLayout(
        geode::ColumnLayout::create()
            ->setAutoGrowAxis(290.f)
            ->setAxisReverse(true)
            ->setGap(0.f)
    );

    for (auto& [section, friends] : InviteManager::get().m_friends) {
        auto sectionInfo = InviteManager::get().m_sourceInfo[section];

        auto sectionHeaderWrapper = cocos2d::CCNode::create();
        sectionHeaderWrapper->setContentSize({ 170.f, 20.f });

        auto sectionLabel = cocos2d::CCLabelBMFont::create(sectionInfo.m_name.c_str(), "bigFont.fnt");
        sectionLabel->setScale(.65f);
        sectionHeaderWrapper->addChildAtPosition(sectionLabel, geode::Anchor::Center);

        auto sectionIcon = sectionInfo.m_icon;
        sectionHeaderWrapper->addChildAtPosition(sectionIcon, geode::Anchor::Left, { 10.f, 0.f });

        scrollLayer->m_contentLayer->addChild(sectionHeaderWrapper);

        for (auto& person : friends) {
            auto cell = FriendCell::create(person);
            scrollLayer->m_contentLayer->addChild(cell);
        }
    }

    scrollLayer->m_contentLayer->updateLayout();
    scrollLayer->scrollToTop();

    m_mainLayer->addChildAtPosition(scrollLayer, geode::Anchor::Center, { 0.f, -11.f });

    // run the animation
    m_noElasticity = true;
    setOpacity(0);

    auto width = m_mainLayer->getContentWidth();
    m_mainLayer->setPositionX(winSize.width + width / 2.f + 20.f);
    m_mainLayer->runAction(
        cocos2d::CCEaseExponentialOut::create(
            cocos2d::CCMoveBy::create(
                .5f,
                { -width - 30.f, 0.f }
            )
        )
    );

    runAction(
        cocos2d::CCFadeTo::create(.4f, 105)
    );
    
    return true;
}

std::pair<std::string, std::string> InviteListingPopup::getInvitePreviewInfo() {
    std::string levelName = "(unknown)";
    std::string lobbyName = "";

    if (auto levelInfoLayer = cocos2d::CCScene::get()->getChildByType<LevelInfoLayer>(0)) {
        levelName = levelInfoLayer->m_level->m_levelName;
    }

    if (auto gjbgl = GJBaseGameLayer::get()) {
        levelName = gjbgl->m_level->m_levelName;
    }

    return { levelName, lobbyName };
}

void InviteListingPopup::onClose(cocos2d::CCObject* sender) {
    if (m_closing) return;
    m_closing = true;

    // animate back out before actually closing
    auto width = m_mainLayer->getContentWidth();
    m_mainLayer->runAction(
        cocos2d::CCSpawn::createWithTwoActions(
            cocos2d::CCEaseExponentialOut::create(
                cocos2d::CCMoveBy::create(
                    .4f,
                    { width + 30.f, 0.f }
                )
            ),
            cocos2d::CCSequence::createWithTwoActions(
                cocos2d::CCDelayTime::create(.3f),
                geode::cocos::CallFuncExt::create([this]() {
                    Popup::onClose(nullptr);
                })
            )
        )
    );

    // fade out background
    runAction(
        cocos2d::CCFadeTo::create(.3f, 0)
    );
}
