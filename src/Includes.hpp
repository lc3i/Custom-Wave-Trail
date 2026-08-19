#pragma once

#include <Geode/ui/GeodeUI.hpp>

using namespace geode::prelude;

template <string::ConstexprString S, typename T>
const T& getSettingFast() {
    static T value = (
        listenForSettingChanges<T>(S.data(), [](T val) {
            value = val;
        }),
        Mod::get()->getSettingValue<T>(S.data())
    );

    return value;
}

inline auto& getPopup() {
    static Popup* s_modPopup = nullptr;
    return s_modPopup;
}

static void toggleSettingsPopup() {
    auto& s_modPopup = getPopup();

    if (!s_modPopup) {
        s_modPopup = openSettingsPopup(Mod::get(), false);
        return;
    }

    if (auto scene = CCScene::get()) {
        for (auto node : scene->getChildrenExt()) {
            if (s_modPopup == node) {
                static_cast<FLAlertLayer*>(node)->keyBackClicked();
                return;
            }
        }
    }

    s_modPopup = openSettingsPopup(Mod::get(), false);
}

static void setHookEnabled(std::string_view name, bool enabled) {
    for (auto hook : Mod::get()->getHooks()) {
        if (hook->getDisplayName() == name) {
            (void)(enabled ? hook->enable() : hook->disable());
            break;
        }
    }
}