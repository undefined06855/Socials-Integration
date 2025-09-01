#pragma once
#include "struct.hpp"

// This is the source file to include if you are a SOURCE mod - i.e. providing
// invite data to the mod

#include <Geode/loader/Dispatch.hpp>
#define MY_MOD_ID "undefined0.socials_integration"

namespace socials {

class ProvideInviteDataEvent : public geode::Event {
    std::unordered_map<std::string, matjson::Value>& m_returnData;

public:
    ProvideInviteDataEvent(std::unordered_map<std::string, matjson::Value>& returnData)
        : m_returnData(returnData) {}
    
    void provide(std::string sourceID, matjson::Value data) {
        if (m_returnData.contains(sourceID)) return;
        m_returnData[sourceID] = data;
    }
};

class ReceiveInviteDataEvent : public geode::Event {
    friend class ReceiveInviteDataFilter;
    std::string m_sourceID;
    matjson::Value m_data;
public:
    ReceiveInviteDataEvent(std::string sourceID, matjson::Value data)
        : m_sourceID(sourceID)
        , m_data(data) {}
    
    matjson::Value getData() { return m_data; }
};

class ReceiveInviteDataFilter : public geode::EventFilter<ReceiveInviteDataEvent> {
    std::string m_sourceID;

public:
    ReceiveInviteDataFilter(std::string sourceID)
        : m_sourceID(sourceID) {}

    using Callback = geode::ListenerResult(ReceiveInviteDataEvent*);
    geode::ListenerResult handle(std::function<Callback> fn, ReceiveInviteDataEvent* event) {
        if (event->m_sourceID != m_sourceID) return geode::ListenerResult::Propagate;
        return fn(event);
    }
};

// events to send to the mod:

class ProvideButtonParentMenuEvent : public geode::Event {
    geode::Ref<cocos2d::CCMenu> m_menu;
    
public:
    ProvideButtonParentMenuEvent(cocos2d::CCMenu* menu)
        : m_menu(menu) {}
    
    cocos2d::CCMenu* getMenu() { return m_menu; }
};

class JoinedRoomEvent : public geode::Event {
public:
    JoinedRoomEvent()
};

class LeftRoomEvent : public geode::Event {

};

}
