#pragma once
#include "struct.hpp"

// This is the header file to include if you are a PROVIDER mod - i.e. providing
// a friends list to the mod

#include <Geode/loader/Dispatch.hpp>
#define MY_MOD_ID "undefined0.socials_integration"

namespace socials {

// friend events

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
    friend class InviteFriendFilter; // so it can access providerID
    std::string m_providerID;
    std::string m_friendID;
    std::string m_inviteData;

public:
    InviteFriendEvent(std::string providerID, std::string friendID, std::string inviteData)
        : m_providerID(providerID)
        , m_friendID(friendID)
        , m_inviteData(inviteData) {}
    
    std::string getFriendID() { return m_friendID; }
    std::string getInviteData() { return m_inviteData; }
};

class InviteFriendFilter : public geode::EventFilter<InviteFriendEvent> {
    std::string m_providerID;

public:
    InviteFriendFilter(std::string providerID)
        : m_providerID(providerID) {}

    using Callback = geode::ListenerResult(InviteFriendEvent*);
    geode::ListenerResult handle(std::function<Callback> fn, InviteFriendEvent* event) {
        if (event->m_providerID != m_providerID) return geode::ListenerResult::Propagate;
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



class RoomJoinedEvent : public geode::Event {

};

class RoomLeftEvent : public geode::Event {

};

}
