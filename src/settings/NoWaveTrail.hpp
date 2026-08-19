#pragma once

#include "base/BoolToggleSetting.hpp"
#include "hooks/HookDependency.hpp"

struct NoWaveTrailTag {};

using NoWaveTrail = BoolToggleValue<NoWaveTrailTag, "separate-no-wave-trail">;

using NoWaveTrailSetting = BoolToggleSetting<
    "no-wave-trail",
    "separate-no-wave-trail",
    "No Wave Trail",
    NoWaveTrail
>;

template <>
struct geode::SettingTypeForValueType<NoWaveTrail> {

    using SettingType = NoWaveTrailSetting;

};

REGISTER_HOOK_DEPENDENCY("no-wave-trail", NoWaveTrail, "PlayerObject::fadeOutStreak2");