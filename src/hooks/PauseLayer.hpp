#pragma once

#include "Includes.hpp"

#include <Geode/modify/PauseLayer.hpp>

class $modify(ProPauseLayer, PauseLayer) {

    struct Fields {
        CCLayerColor* backTrail = nullptr;
        CCLayerColor* frontTrail = nullptr;
        float pulseTime = 0.f; 
        float rgbTime = 0.f; 
    };

    void applyTrailSettings();
    void updateTrail(float);

    $override
    void customSetup();

};