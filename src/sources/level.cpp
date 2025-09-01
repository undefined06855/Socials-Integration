#include "source.hpp"

$on_mod(Loaded) {
    // listen to ProvideInviteDataEvent
    // listen to ReceiveInviteDataFilter<ReceiveInviteDataEvent>
    // send out ProvideButtonParentMenuEvent

    new geode::EventListener<geode::EventFilter<socials::ProvideInviteDataEvent>>(+[](socials::ProvideInviteDataEvent* event) {
        return geode::ListenerResult::Propagate;
    });

    new geode::EventListener<socials::ReceiveInviteDataFilter>(+[](socials::ReceiveInviteDataEvent* event) {
        return geode::ListenerResult::Propagate;
    }, socials::ReceiveInviteDataFilter("level"_spr));
}
