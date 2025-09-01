#pragma once

namespace socials {

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

struct RoomInfo {
    std::string m_name;
    std::string m_sourceID;
    std::string m_password;

    int m_priority; // rooms with higher priority overrides rooms with lower priorities    
};

}
