#pragma once

#include "base/BoolToggleSetting.hpp"
#include "hooks/HookDependency.hpp"

struct NoWavePulseTag {};

using NoWavePulse = BoolToggleValue<NoWavePulseTag, "separate-no-wave-pulse">;

using NoWavePulseSetting = BoolToggleSetting<
    "no-wave-pulse",
    "separate-no-wave-pulse",
    "No Wave Pulse",
    NoWavePulse
>;

template <>
struct geode::SettingTypeForValueType<NoWavePulse> {

    using SettingType = NoWavePulseSetting;

};

REGISTER_HOOK_DEPENDENCY("no-wave-pulse", NoWavePulse, "PlayerObject::update");