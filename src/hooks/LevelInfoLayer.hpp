#pragma once
#include <Geode/modify/LevelInfoLayer.hpp>

class $modify(HookedLevelInfoLayer, LevelInfoLayer) {
    bool init(GJGameLevel* level, bool challenge);
};
