#pragma once

#include "Includes.hpp"

#include "base/BaseSettingConfig.hpp"
#include "base/BaseSettingSubNode.hpp"
#include "hooks/HookDependency.hpp"

#include <Geode/loader/SettingV3.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/ui/Button.hpp>
#include <Geode/ui/SliderNode.hpp>

struct WavePulseSizeConfig {
    float size = 1.f;
    bool enabled = false;
  
    bool operator==(const WavePulseSizeConfig&) const = default;
};

using WavePulseSize = BaseSettingConfig<WavePulseSizeConfig, "separate-wave-pulse-size">;

template <>
struct matjson::Serialize<WavePulseSize> {
    
    static matjson::Value toJson(const WavePulseSize& value) {
        auto obj = matjson::Value{};
        auto sub = matjson::Value{};

        sub["size"] = value.p1Config.size;
        sub["enabled"] = value.p1Config.enabled;

        obj["p1"] = sub;

        sub = matjson::Value{};

        sub["size"] = value.p2Config.size;
        sub["enabled"] = value.p2Config.enabled;

        obj["p2"] = sub;

        return obj;
    }

    static Result<WavePulseSize> fromJson(const matjson::Value& value) {
        return Ok(WavePulseSize{
            WavePulseSizeConfig{
                static_cast<float>(value["p1"]["size"].asDouble().unwrapOr(1.f)),
                value["p1"]["enabled"].asBool().unwrapOr(false)
            },
            WavePulseSizeConfig{
                static_cast<float>(value["p2"]["size"].asDouble().unwrapOr(1.f)),
                value["p2"]["enabled"].asBool().unwrapOr(false)
            }
        });
    }

};

class WavePulseSizeSetting : public SettingBaseValueV3<WavePulseSize> {

public:

    static Result<std::shared_ptr<SettingV3>> parse(const std::string&, const std::string&, const matjson::Value&); 

    SettingNodeV3* createNode(float) override;

};

template <>
struct SettingTypeForValueType<WavePulseSize> {
    using SettingType = WavePulseSizeSetting;
};

class WavePulseSizeSettingNode : public BaseSettingSubNode<WavePulseSizeConfig> {

private:

    TextInput* m_input = nullptr;
    SliderNode* m_slider = nullptr;
    CCLabelBMFont* m_label = nullptr;
    bool m_ignoreCallback = false;

    WavePulseSizeSettingNode(std::string name)
        : BaseSettingSubNode(std::move(name)) {}

    bool init(float);

    void setValue(const WavePulseSizeConfig&) override;
    void onReset() override;
    void setEnabled(bool) override;

public:

    static WavePulseSizeSettingNode* create(float);

};

REGISTER_HOOK_DEPENDENCY("wave-pulse-size", WavePulseSize, "PlayerObject::update");