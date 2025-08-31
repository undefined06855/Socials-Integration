#include "LevelInfoLayer.hpp"
#include "../ui/InviteListingPopup.hpp"

bool HookedLevelInfoLayer::init(GJGameLevel* level, bool challenge) {
    if (!LevelInfoLayer::init(level, challenge)) return false;

    auto menu = getChildByID("other-menu");
    if (!menu) return true;

    auto listButton = menu->getChildByID("list-button");
    if (!listButton) return true;

    auto button = geode::cocos::CCMenuItemExt::createSpriteExtraWithFilename(
        "invite-button.png"_spr, .65f,
        [](auto) {
            InviteListingPopup::create()->show();
        }
    );
    button->setPositionX(listButton->getPositionX());
    button->setPositionY(listButton->getPositionY() + 35.f);
    menu->addChild(button);
    
    return true;
}
