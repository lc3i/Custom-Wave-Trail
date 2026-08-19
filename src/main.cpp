#include "Includes.hpp"

#include "settings/WaveColor.hpp"
#include "settings/WaveRGB.hpp"
#include "settings/SolidWaveTrail.hpp"
#include "settings/WaveTrailSize.hpp"
#include "settings/WavePulseSize.hpp"
#include "settings/NoWavePulse.hpp"
#include "settings/NoWaveTrail.hpp"
#include "settings/KeepTrailOnDeath.hpp"
#include "settings/NoTrailBehind.hpp"

#include "hooks/PlayerObject.hpp"
#include "hooks/PlayLayer.hpp"
#include "hooks/PauseLayer.hpp"
#include "hooks/HookDependency.hpp"

void updateEnabledHooks() {
    auto hooks = std::unordered_map<std::string_view, bool>{};
    
    for (const auto& dep : hookDependencies()) {
        if (dep.isEnabled()) {
            hooks[dep.hook] = true;
        } else if (!hooks.contains(dep.hook)) {
            hooks[dep.hook] = false;
        }
    }

    for (const auto& [hook, enabled] : hooks) {
        setHookEnabled(hook, enabled);
    }
}

$on_mod(Loaded) {
    (void)Mod::get()->registerCustomSettingType("wave-color", &WaveColorSetting::parse);
    (void)Mod::get()->registerCustomSettingType("wave-rgb", &WaveRGBSetting::parse);
    (void)Mod::get()->registerCustomSettingType("solid-wave-trail", &SolidWaveTrailSetting::parse);
    (void)Mod::get()->registerCustomSettingType("wave-trail-size", &WaveTrailSizeSetting::parse);
    (void)Mod::get()->registerCustomSettingType("wave-pulse-size", &WavePulseSizeSetting::parse);
    (void)Mod::get()->registerCustomSettingType("no-wave-pulse", &NoWavePulseSetting::parse);
    (void)Mod::get()->registerCustomSettingType("no-wave-trail", &NoWaveTrailSetting::parse);
    (void)Mod::get()->registerCustomSettingType("keep-trail-on-death", &KeepTrailOnDeathSetting::parse);
    (void)Mod::get()->registerCustomSettingType("no-trail-behind", &NoTrailBehindSetting::parse);

    listenForAllSettingChanges([](std::string_view, std::shared_ptr<SettingV3>) {
        if (auto pl = PlayLayer::get()) {
            static_cast<ProPlayLayer*>(pl)->applyPlayerSettings();
        }

        if (auto scene = CCScene::get()) {
            if (auto pause = scene->getChildByType<PauseLayer>(0)) {
                static_cast<ProPauseLayer*>(pause)->applyTrailSettings();
            }
        }
    
        updateEnabledHooks();
    });

    listenForKeybindSettingPresses("open-menu", [](Keybind const& keybind, bool down, bool repeat, double timestamp) {
        if (down && !repeat) {
            toggleSettingsPopup();
        }
    });

    updateEnabledHooks();
}
