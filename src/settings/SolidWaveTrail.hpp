#pragma once

#include "base/BoolToggleSetting.hpp"
#include "hooks/HookDependency.hpp"

struct SolidWaveTrailTag {};

using SolidWaveTrail = BoolToggleValue<SolidWaveTrailTag, "separate-solid-wave-trail">;

using SolidWaveTrailSetting = BoolToggleSetting<
    "solid-wave-trail",
    "separate-solid-wave-trail",
    "Solid Wave Trail",
    SolidWaveTrail
>;

template <>
struct geode::SettingTypeForValueType<SolidWaveTrail> {

    using SettingType = SolidWaveTrailSetting;

};

REGISTER_HOOK_DEPENDENCY("solid-wave-trail", SolidWaveTrail, "PlayerObject::updateStreakBlend");
REGISTER_HOOK_DEPENDENCY("solid-wave-trail", SolidWaveTrail, "PlayerObject::setupStreak");
REGISTER_HOOK_DEPENDENCY("solid-wave-trail", SolidWaveTrail, "PlayerObject::resetStreak");