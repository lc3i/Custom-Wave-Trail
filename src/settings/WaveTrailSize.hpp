#pragma once

#include "Includes.hpp"

#include "base/BaseSettingConfig.hpp"
#include "base/BaseSettingSubNode.hpp"
#include "hooks/HookDependency.hpp"

#include <Geode/loader/SettingV3.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/ui/Button.hpp>
#include <Geode/ui/SliderNode.hpp>

struct WaveTrailSizeConfig {
    float size = 1.f;
    bool enabled = false;
  
    bool operator==(const WaveTrailSizeConfig&) const = default;
};

using WaveTrailSize = BaseSettingConfig<WaveTrailSizeConfig, "separate-wave-trail-size">;

template <>
struct matjson::Serialize<WaveTrailSize> {
    
    static matjson::Value toJson(const WaveTrailSize& value) {
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

    static Result<WaveTrailSize> fromJson(const matjson::Value& value) {
        return Ok(WaveTrailSize{
            WaveTrailSizeConfig{
                static_cast<float>(value["p1"]["size"].asDouble().unwrapOr(1.f)),
                value["p1"]["enabled"].asBool().unwrapOr(false)
            },
            WaveTrailSizeConfig{
                static_cast<float>(value["p2"]["size"].asDouble().unwrapOr(1.f)),
                value["p2"]["enabled"].asBool().unwrapOr(false)
            }
        });
    }

};

class WaveTrailSizeSetting : public SettingBaseValueV3<WaveTrailSize> {

public:

    static Result<std::shared_ptr<SettingV3>> parse(const std::string&, const std::string&, const matjson::Value&); 

    SettingNodeV3* createNode(float) override;

};

template <>
struct SettingTypeForValueType<WaveTrailSize> {
    using SettingType = WaveTrailSizeSetting;
};

class WaveTrailSizeSettingNode : public BaseSettingSubNode<WaveTrailSizeConfig> {

private:

    TextInput* m_input = nullptr;
    SliderNode* m_slider = nullptr;
    CCLabelBMFont* m_label = nullptr;
    bool m_ignoreCallback = false;

    WaveTrailSizeSettingNode(std::string name)
        : BaseSettingSubNode(std::move(name)) {}

    bool init(float);

    void setValue(const WaveTrailSizeConfig&) override;
    void onReset() override;
    void setEnabled(bool) override;

public:

    static WaveTrailSizeSettingNode* create(float);

};

REGISTER_HOOK_DEPENDENCY("wave-trail-size", WaveTrailSize, "PlayerObject::togglePlayerScale");