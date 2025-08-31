#include "FriendCell.hpp"
#include "../InviteManager.hpp"

FriendCell* FriendCell::create(socials::Friend &person) {
    auto ret = new FriendCell;
    if (ret->init(person)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}

bool FriendCell::init(socials::Friend& person) {
    if (!CCNode::init()) return false;

    m_friendInfo = person;
    setContentSize({ 170.f, 20.f });

    auto clip = cocos2d::CCLayerColor::create();
    clip->setAnchorPoint({ .5f, .5f });
    clip->ignoreAnchorPointForPosition(false); // FUCK YOU!!!!!
    clip->setContentSize({ 60.f, getContentHeight() });
    // actually important properties:
    clip->setSkewX(10.f);
    clip->setPositionY(getContentHeight() / 2.f); // center vertically
    clip->setPositionX(24.f);

    auto clippingNode = cocos2d::CCClippingNode::create(clip);
    clippingNode->setAnchorPoint({ .5f, .5f });
    clippingNode->setContentSize(getContentSize());

    person.m_avatar->setColor({ 184, 184, 184 });
    clippingNode->addChildAtPosition(person.m_avatar, geode::Anchor::Left, { 28.f, 0.f });

    addChildAtPosition(clippingNode, geode::Anchor::Center);

    auto menu = cocos2d::CCMenu::create();
    menu->ignoreAnchorPointForPosition(false);
    addChildAtPosition(menu, geode::Anchor::Right);

    auto label = cocos2d::CCLabelBMFont::create(m_friendInfo.m_name.c_str(), "bigFont.fnt");
    label->setScale(.5f);
    label->setAnchorPoint({ 0.f, .5f });
    addChildAtPosition(label, geode::Anchor::Left, { 5.f, 0.f });

    auto button = geode::cocos::CCMenuItemExt::createSpriteExtraWithFilename(
        "invite-button.png"_spr, .4f,
        [this](auto) {
            InviteManager::get().invite(m_friendInfo.m_source, m_friendInfo.m_id);
        }
    );
    menu->addChildAtPosition(button, geode::Anchor::Center, { -10.f, 0.f });

    return true;
}
