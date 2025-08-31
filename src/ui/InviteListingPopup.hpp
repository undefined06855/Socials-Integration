#pragma once

class InviteListingPopup : public geode::Popup<> {
public:
    static InviteListingPopup* create();
    bool setup();

    bool m_closing;

    std::pair<std::string, std::string> getInvitePreviewInfo();

    void onClose(cocos2d::CCObject* sender);
};
