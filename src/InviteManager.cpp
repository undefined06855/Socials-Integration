#include "InviteManager.hpp"
#include "LoadLevelPopup.hpp"
#include "provider.hpp"

InviteManager::InviteManager()
    : m_friends({})
    , m_sourceInfo({}) {}

InviteManager& InviteManager::get() {
    static InviteManager instance;
    return instance;
}

void InviteManager::onInviteAccepted(const std::string& data) {
    auto json = matjson::parse(data).unwrapOr("");

    if (!json.isObject()) {
        geode::log::warn("Accepted join with invalid data!");
        return;
    }

    geode::log::info("{}", json.dump());

    // do {
    //     if (auto levelData = json.get("level")) {
    //         auto unwrapped = levelData.unwrap();
    //         auto dailyValue = unwrapped["daily"].asUInt().unwrapOr(0);
    //         if (dailyValue != 0) {
    //             if (dailyValue > 200000) {
    //                 DailyLevelPage::create(GJTimedLevelType::Event)->show();
    //             } else if (dailyValue > 100000) {
    //                 DailyLevelPage::create(GJTimedLevelType::Weekly)->show();
    //             } else {
    //                 DailyLevelPage::create(GJTimedLevelType::Daily)->show();
    //             }

    //             break;
    //         }

    //         auto levelID = unwrapped["id"].asUInt().unwrapOr(0);
    //         if (levelID != 0) {
    //             auto pop = LoadLevelPopup::create(levelID);
    //             if (!pop) break;
    //             pop->show();
    //         }
    //     }
    // } while (0);

    // do {
    //     if (auto globedData = json.get("globed")) {
    //         auto unwrapped = globedData.unwrap();

    //         if (!globed::isLoaded()) {
    //             geode::log::info("invite data had globed but globed not loaded!");
    //             break;
    //         }

    //         if (!globed::net::isConnected().unwrapOr(false)) {
    //             geode::log::warn("globed not yet connected but invite accepted!");
    //             FLAlertLayer::create(
    //                 "Socials Integration - Globed",
    //                 "Could not join <co>Globed</c> room since you are "
    //                 "<cr>not</c> yet connected to a <cj>server</c>!",
    //                 "ok"
    //             )->show();
    //             break;
    //         }

    //         auto id = unwrapped["id"].asUInt().unwrapOr(0);
    //         auto pass = unwrapped["pass"].asString().unwrapOr("");
    //         (void)globed::room::joinRoom(id, pass);
    //     }
    // } while (0);
}

// called by the buttons
// pass in the source of the friend and the id of the friend
void InviteManager::invite(std::string source, std::string id) {
    auto json = matjson::makeObject({});

    if (auto layer = cocos2d::CCScene::get()->getChildByType<LevelInfoLayer>(0)) {
        auto data = matjson::makeObject({});

        data["id"] = layer->m_level->m_levelID.value();
        data["daily"] = layer->m_level->m_dailyID.value();

        json["level"] = data;
    } else if (auto layer = GJBaseGameLayer::get()) {
        auto data = matjson::makeObject({});

        data["id"] = layer->m_level->m_levelID.value();
        data["daily"] = layer->m_level->m_dailyID.value();

        json["level"] = data;
    }

    // if (auto res = globed::room::getRoomData()) {
    //     auto data = matjson::makeObject({});
    //     auto room = res.unwrap();

    //     data["id"] = room.id;
    //     data["pass"] = room.password;

    //     json["globed"] = data;
    // }

    auto jsonString = json.dump(matjson::NO_INDENTATION);

    socials::InviteFriendEvent(source, id, jsonString).post();
}
