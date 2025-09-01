#define GEODE_DEFINE_EVENT_EXPORTS
#include "provider.hpp"
#include "source.hpp"
#include "InviteManager.hpp"

geode::Result<> socials::GatherFriendsEvent::submitFriendSource(std::vector<socials::Friend>& friends, socials::SourceInfo info) {
    for (auto& person : friends) {
        person.m_source = info.m_id;
    }

    InviteManager::get().m_friends[info.m_id] = friends;
    InviteManager::get().m_sourceInfo[info.m_id] = info;

    return geode::Ok();
}
