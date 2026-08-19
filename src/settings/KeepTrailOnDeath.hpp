#pragma once

#include "base/BoolToggleSetting.hpp"
#include "hooks/HookDependency.hpp"

struct KeepTrailOnDeathTag {};

using KeepTrailOnDeath = BoolToggleValue<KeepTrailOnDeathTag, "separate-no-wave-pulse">;

using KeepTrailOnDeathSetting = BoolToggleSetting<
    "keep-trail-on-death",
    "separate-keep-trail-on-death",
    "Keep Trail On Death",
    KeepTrailOnDeath
>;

template <>
struct geode::SettingTypeForValueType<KeepTrailOnDeath> {

    using SettingType = KeepTrailOnDeathSetting;

};

REGISTER_HOOK_DEPENDENCY("keep-trail-on-death", KeepTrailOnDeath, "PlayerObject::fadeOutStreak2");