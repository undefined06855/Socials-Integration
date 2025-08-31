#pragma once
#pragma clang diagnostic ignored "-Wdeprecated-declarations" // steamapi uses a strncopy
#include <steamworks/isteamfriends.h>
#include <steamworks/isteamutils.h>

class SteamAvatarLoader {
public:
    static SteamAvatarLoader& get();

    std::unordered_map<uint64_t, geode::Ref<geode::LazySprite>> m_loadQueue;

    geode::LazySprite* loadAvatar(CSteamID steamID);
    void loadAvatarRaw(geode::LazySprite* sprite, int handle);
};

class SteamCallbackListener {
public:
    STEAM_CALLBACK(SteamCallbackListener, onJoinAccepted, GameRichPresenceJoinRequested_t);
    STEAM_CALLBACK(SteamCallbackListener, onAvatarLoaded, AvatarImageLoaded_t);
};

// to allow scheduler
class SteamCallbackRunner : public cocos2d::CCObject {
public:
    void runCallbacks(float dt);
};
