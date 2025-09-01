#include "MenuLayer.hpp"
#include "provider.hpp"

bool HookedMenuLayer::init() {
    if (!MenuLayer::init()) return false;

    // get friend sources from mods
    static bool fetched = false;
    if (!fetched) {
        fetched = true;
        socials::GatherFriendsEvent().post();
    }

    return true;
}
