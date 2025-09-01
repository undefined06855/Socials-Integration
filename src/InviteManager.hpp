#pragma once
#include "provider.hpp"
#include "source.hpp"

class InviteManager {
    InviteManager();
public:
    static InviteManager& get();

    std::unordered_map<std::string, std::vector<socials::Friend>> m_friends;
    std::unordered_map<std::string, socials::SourceInfo> m_sourceInfo;

    

    void onInviteAccepted(const std::string& data);
    void invite(std::string source, std::string id);
};
