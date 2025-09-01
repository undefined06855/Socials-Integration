#include "provider.hpp"
#include "InviteManager.hpp"

$on_mod(Loaded) {
    // listen to mods being detected that they've been invited
    new geode::EventListener<geode::EventFilter<socials::InviteAcceptedEvent>>(+[](socials::InviteAcceptedEvent* event) {
        InviteManager::get().onInviteAccepted(event->getInviteData());
        return geode::ListenerResult::Stop;
    });
}

#include <Geode/modify/MenuLayer.hpp>
#include "LoadLevelPopup.hpp"
class $modify(MenuLayer) {
    void onMoreGames(cocos2d::CCObject*) {
        LoadLevelPopup::create(124469015)->show();
    }
};
