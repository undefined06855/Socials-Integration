#pragma once
#include "events.hpp"

class FriendCell : public cocos2d::CCNode {
public:
    static FriendCell* create(socials::Friend& person);
    bool init(socials::Friend& person);

    socials::Friend m_friendInfo;
};
