#pragma once

#include "Includes.hpp"

template <typename Config, string::ConstexprString SeparateId>
struct BaseSettingConfig {
    Config p1Config;
    Config p2Config;

    bool operator==(const BaseSettingConfig&) const = default;

    explicit operator bool() const {
        return p1Config.enabled || p2Config.enabled;
    }

    const Config& get(bool player1) const {
        if (player1 || !getSettingFast<SeparateId, bool>()) {
            return p1Config;
        }
        return p2Config;
    }
};