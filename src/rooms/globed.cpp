// #include <dankmeme.globed2/include/globed.hpp>
// #include "provider.hpp"

// socials::RoomInfo globedToSocials(globed::RoomData data) {
//     return {
//         .m_roomSource = "globed",
//         .m_roomID = data.id,
//         .m_roomPassword = data.password,
//         .m_maxPlayerCount = data.playerLimit,

//         .m_public = data.isHidden,
//         .m_canInvite = data.openInvites || (data.ownerAccountId == GJAccountManager::get()->m_accountID)
//     };
// }

// $on_mod(Loaded) {
//     new geode::EventListener<geode::EventFilter<globed::NetworkManagerConnectedEvent>>(+[](globed::NetworkManagerConnectedEvent* event) {
//         geode::log::info("networkmanager connected detected");

//         // only online room so far but if more are added (cr etc) move this out of here
//         socials::NetworkReadyEvent().post();

//         return geode::ListenerResult::Propagate;
//     });

//     new geode::EventListener<geode::EventFilter<globed::NetworkManagerDisconnectedEvent>>(+[](globed::NetworkManagerDisconnectedEvent* event) {
//         geode::log::info("networkmanager disconnect detected");

//         return geode::ListenerResult::Propagate;
//     });

//     new geode::EventListener<geode::EventFilter<globed::RoomJoinEvent>>(+[](globed::RoomJoinEvent* event) {
//         geode::log::info("joined GLOBED room {}", event->getRoomData().name);

//         socials::RoomJoinEvent(globedToSocials(event->getRoomData())).post();

//         return geode::ListenerResult::Propagate;
//     });

//     new geode::EventListener<geode::EventFilter<globed::RoomUpdateEvent>>(+[](globed::RoomUpdateEvent* event) {
//         geode::log::info("updated GLOBED room {}", event->getRoomData().name);

//         socials::RoomUpdateEvent(globedToSocials(event->getRoomData())).post();

//         return geode::ListenerResult::Propagate;
//     });

//     new geode::EventListener<geode::EventFilter<globed::RoomLeaveEvent>>(+[](globed::RoomLeaveEvent* event) {
//         geode::log::info("left GLOBED room");

//         socials::RoomLeftEvent("globed").post();

//         return geode::ListenerResult::Propagate;
//     });
// }
