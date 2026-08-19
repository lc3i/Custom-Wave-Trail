#pragma once

#include "base/BoolToggleSetting.hpp"
#include "hooks/HookDependency.hpp"

struct NoTrailBehindTag {};

using NoTrailBehind = BoolToggleValue<NoTrailBehindTag, "separate-no-trail-behind">;

using NoTrailBehindSetting = BoolToggleSetting<
    "no-trail-behind",
    "separate-no-trail-behind",
    "No Trail Behind",
    NoTrailBehind
>;

template <>
struct geode::SettingTypeForValueType<NoTrailBehind> {

    using SettingType = NoTrailBehindSetting;

};

REGISTER_HOOK_DEPENDENCY("no-trail-behind", NoTrailBehind, "PlayerObject::switchedToMode");