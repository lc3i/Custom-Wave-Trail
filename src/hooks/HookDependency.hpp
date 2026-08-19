#pragma once

#include "Includes.hpp"

struct HookDependency {
    std::string_view hook;
    std::function<bool()> isEnabled;
};

inline std::vector<HookDependency>& hookDependencies() {
    static std::vector<HookDependency> deps;
    return deps;
}

#define HOOK_DEP_CONCAT_IMPL(a, b) a##b
#define HOOK_DEP_CONCAT(a, b) HOOK_DEP_CONCAT_IMPL(a, b)
#define REGISTER_HOOK_DEPENDENCY(settingId, type, hook) \
    inline bool HOOK_DEP_CONCAT(hookDependencyReg_, __COUNTER__) = (hookDependencies().push_back({hook, [] { return static_cast<bool>(getSettingFast<settingId, type>()); }}), true)