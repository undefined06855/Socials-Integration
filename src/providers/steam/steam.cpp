#include "provider.hpp"
#include "steamapi.hpp"

$on_mod(Loaded) {
    // on mod loaded
    new geode::EventListener<geode::EventFilter<socials::GatherFriendsEvent>>(+[](socials::GatherFriendsEvent* event) {
        std::vector<socials::Friend> list = {};

        // TODO: is all friends the best way to do this? includes blocked and stuff
        auto friendCount = SteamFriends()->GetFriendCount(EFriendFlags::k_EFriendFlagImmediate);
    
        for (int i = 0; i < friendCount; i++) {
            auto current = SteamFriends()->GetFriendByIndex(i, EFriendFlags::k_EFriendFlagImmediate);

            if (current == k_steamIDNil) continue;

            auto avatar = SteamAvatarLoader::get().loadAvatar(current);
            avatar->setScale(1.5f);

            list.push_back(socials::Friend {
                .m_name = SteamFriends()->GetFriendPersonaName(current),
                .m_id = fmt::to_string(current.ConvertToUint64()),
                .m_avatar = avatar
            });
        }

        auto icon = cocos2d::CCSprite::create("steam-icon.png"_spr);
        icon->setScale(.5f);
        auto info = socials::SourceInfo {
            .m_name = "Steam",
            .m_id = "steam"_spr,
            .m_icon = icon,
        };

        (void)event->submitFriendSource(list, info);

        return geode::ListenerResult::Propagate;
    });


    // when the invite button is clicked in the ui
    new geode::EventListener<socials::InviteFriendFilter>(+[](socials::InviteFriendEvent* event) {
        auto id = event->getFriendID();
        auto data = event->getInviteData();

        // escape all quotes and add prefix
        data = geode::utils::string::replace(data, "\"", "\\\"");
        auto args = fmt::format("--geode:steam_invite_data=\"{}\"", data);
        
        SteamFriends()->InviteUserToGame(CSteamID(geode::utils::numFromString<uint64_t>(id).unwrapOr(0)), args.c_str());

        return geode::ListenerResult::Stop;
    }, socials::InviteFriendFilter("steam"_spr));


    // when network is ready to potentially join a room
    new geode::EventListener<geode::EventFilter<socials::NetworkReadyEvent>>(+[](socials::NetworkReadyEvent* event) {
        // check launch invites
        auto arg = geode::Loader::get()->getLaunchArgument("steam_invite_data");
        if (!arg) return geode::ListenerResult::Propagate;
    
        auto quoteWrappedString = arg.value();
        auto string = matjson::parse(quoteWrappedString).unwrapOr("").asString().unwrapOr("");
    
        // unescape quotes
        string = geode::utils::string::replace(string, "\\\"", "\"");
    
        socials::InviteAcceptedEvent(string).post();

        return geode::ListenerResult::Propagate;
    });
}

// when we detect we've been invited through our pathway, call InviteAcceptedEvent
// to let Socials Integration put the player where they should be,
// this is while the game is running
void SteamCallbackListener::onJoinAccepted(GameRichPresenceJoinRequested_t* pParam) {
    // strip the steam invite data
    constexpr int offset = std::string_view("--geode:steam_invite_data=").length();

    auto quoteWrappedString = std::string_view(pParam->m_rgchConnect + offset);
    auto string = matjson::parse(quoteWrappedString).unwrapOr("").asString().unwrapOr("");

    // unescape quotes
    string = geode::utils::string::replace(string, "\\\"", "\"");

    socials::InviteAcceptedEvent(string).post();
}
