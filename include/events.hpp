#pragma once
#include <Geode/loader/Dispatch.hpp>

#define MY_MOD_ID "undefined0.socials_integration"

namespace socials {

// friend events

struct Friend {
    std::string m_name;
    std::string m_id;
    std::string m_source;
    geode::Ref<geode::LazySprite> m_avatar;
};

struct SourceInfo {
    std::string m_name;
    std::string m_id;
    geode::Ref<cocos2d::CCSprite> m_icon;
};

// sent by this mod on mod load to gather all the available friends
class GatherFriendsEvent : public geode::Event {
public:
    inline geode::Result<> submitFriendSource(std::vector<Friend>& friends, SourceInfo info)
        GEODE_EVENT_EXPORT(&GatherFriendsEvent::submitFriendSource, (this, friends, info));
};

// sent by this mod when all room providers are ready to potentially be joined
// once this is received, check launch invites
class NetworkReadyEvent : public geode::Event {

};

// sent by this mod when the invite button is clicked
class InviteFriendEvent : public geode::Event {
    friend class InviteFriendFilter; // so it can access sourceID
    std::string m_sourceID;
    std::string m_friendID;
    std::string m_inviteData;

public:
    InviteFriendEvent(std::string sourceID, std::string friendID, std::string inviteData)
        : m_sourceID(sourceID)
        , m_friendID(friendID)
        , m_inviteData(inviteData) {}
    
    std::string getFriendID() { return m_friendID; }
    std::string getInviteData() { return m_inviteData; }
};

class InviteFriendFilter : public geode::EventFilter<InviteFriendEvent> {
    std::string m_sourceID;

public:
    InviteFriendFilter(std::string sourceID)
        : m_sourceID(sourceID) {}

    using Callback = geode::ListenerResult(InviteFriendEvent*);
    geode::ListenerResult handle(std::function<Callback> fn, InviteFriendEvent* event) {
        if (event->m_sourceID != m_sourceID) return geode::ListenerResult::Propagate;
        return fn(event);
    }
};

// sent by YOUR mod when it detects the game has launched / the game has been
// sent an invite
class InviteAcceptedEvent : public geode::Event {
    std::string m_inviteData;
public:
    InviteAcceptedEvent(std::string inviteData)
        : m_inviteData(inviteData) {}
    
    std::string getInviteData() { return m_inviteData; }
};

// room events

struct RoomInfo {
    std::string m_roomSource;
    uint64_t m_roomID;
    std::string m_roomPassword;
    uint64_t m_maxPlayerCount;

    bool m_public;
    bool m_canInvite;
};

class RoomJoinEvent : public geode::Event {
    RoomInfo m_roomInfo;

public:
    RoomJoinEvent(const RoomInfo& roomInfo)
        : m_roomInfo(roomInfo) {}

    RoomInfo getRoomInfo() { return m_roomInfo; }
};

class RoomLeftEvent : public geode::Event {
    std::string m_roomSource;

public:
    RoomLeftEvent(std::string roomSource)
        : m_roomSource(roomSource) {}

    std::string getRoomSource() { return m_roomSource; }
};

class RoomUpdateEvent : public geode::Event {
    RoomInfo m_roomInfo;

public:
    RoomUpdateEvent(const RoomInfo& roomInfo)
        : m_roomInfo(roomInfo) {}

    RoomInfo getRoomInfo() { return m_roomInfo; }
};

}
