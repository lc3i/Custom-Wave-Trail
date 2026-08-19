#pragma once

#include "Includes.hpp"

#include <Geode/modify/PlayerObject.hpp>

class $modify(ProPlayerObject, PlayerObject) {

    struct Fields {
        bool didSetColor = false;
        ccColor3B originalColor;

        bool didScheduleRGB = false;
        float rgbTime = 0.f;

        bool didSetSolid = false;
        bool wasSolid = false;
        ccBlendFunc originalBlendFunc;

        bool didSetTrailSize = false;
        float originalTrailSize = 1.f;

        bool didHideTrail = false;
    };

    void applyAllSettings(bool = false);
    void applyTrailColor();
    void applyTrailRGB();
    void applyNoWaveTrail();
    void applyNoWavePulse();
    void applySolidWaveTrail();
    void applyWavePulseSize();
    void applyWaveTrailSize();
    void applyNoTrailBehind();

    void updateTrailRGB(float);

    $override
    void update(float); 
    void updateStreakBlend(bool);
    void setupStreak();
    void resetStreak();
    void fadeOutStreak2(float);
    void togglePlayerScale(bool, bool);
    void switchedToMode(GameObjectType);

};