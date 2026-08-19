#pragma once

#include "Includes.hpp"

#include <Geode/modify/PlayLayer.hpp>

class $modify(ProPlayLayer, PlayLayer) {

    void applyPlayerSettings(bool = false);

    $override
    void setupHasCompleted();
    void resetLevel();

};