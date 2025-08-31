#include "steamapi.hpp"

$on_mod(Loaded) {
    // listen to callbacks globally (dont let this get deleted)
    new SteamCallbackListener();

    // run callbacks every .5s
    cocos2d::CCScheduler::get()->scheduleSelector(schedule_selector(SteamCallbackRunner::runCallbacks), new SteamCallbackRunner, .5f, false);
}

void SteamCallbackRunner::runCallbacks(float dt) {
    SteamAPI_RunCallbacks();
}

SteamAvatarLoader& SteamAvatarLoader::get() {
    static SteamAvatarLoader instance;
    return instance;
}

geode::LazySprite* SteamAvatarLoader::loadAvatar(CSteamID friendID) {
    auto sprite = geode::LazySprite::create({ 184.f, 184.f });

    auto handle = SteamFriends()->GetLargeFriendAvatar(friendID);

    if (handle == -1) {
        m_loadQueue[friendID.ConvertToUint64()] = sprite;
    } else {
        geode::log::debug("Avatar for {} synchronously loaded!", friendID.ConvertToUint64());
        loadAvatarRaw(sprite, handle);
    }

    return sprite;
}

void SteamAvatarLoader::loadAvatarRaw(geode::LazySprite* sprite, int handle) {
    static constexpr int size = 184 * 184 * 4;
    uint8_t data[size] = {};

    if (!SteamUtils()->GetImageRGBA(handle, data, size)) {
        geode::log::warn("Could not get image data for avatar!");
        return; // wont ever load
    }

    // lazysprite loadWithRawData doesn't work?
    auto image = new cocos2d::CCImage;
    image->initWithImageData(data, size, cocos2d::CCImage::kFmtRawData, 184, 184);

    auto texture = new cocos2d::CCTexture2D;
    texture->initWithImage(image);

    image->release();

    auto rect = cocos2d::CCRect{ 0.f, 0.f, 184.f, 184.f };
    rect.size /= cocos2d::CCDirector::get()->getContentScaleFactor();

    sprite->initWithTexture(texture, rect, false);
}

void SteamCallbackListener::onAvatarLoaded(AvatarImageLoaded_t* pParam) {
    auto& loader = SteamAvatarLoader::get();
    if (!loader.m_loadQueue.contains(pParam->m_steamID.ConvertToUint64())) {
        geode::log::warn("Avatar data for {} lazy loaded, but no LazySprite to take it!", pParam->m_steamID.ConvertToUint64());
        return;
    }

    geode::log::debug("Avatar for {} lazy loaded!", pParam->m_steamID.ConvertToUint64());

    auto sprite = loader.m_loadQueue[pParam->m_steamID.ConvertToUint64()];
    loader.loadAvatarRaw(sprite, pParam->m_iImage);

    loader.m_loadQueue.erase(pParam->m_steamID.ConvertToUint64());
}
