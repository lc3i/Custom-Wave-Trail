#pragma once

#include "Includes.hpp"

#include "base/BaseSettingConfig.hpp"
#include "base/BaseSettingSubNode.hpp"

#include <Geode/loader/SettingV3.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/ui/Button.hpp>

struct WaveColorConfig {
    ccColor3B color = {255, 255, 255};
    bool enabled = false;
  
    bool operator==(const WaveColorConfig&) const = default;
};

using WaveColor = BaseSettingConfig<WaveColorConfig, "separate-wave-color">;

template <>
struct matjson::Serialize<WaveColor> {
    
    static matjson::Value toJson(const WaveColor& value) {
        auto obj = matjson::Value{};
        auto sub = matjson::Value{};

        sub["color"] = value.p1Config.color;
        sub["enabled"] = value.p1Config.enabled;

        obj["p1"] = sub;

        sub = matjson::Value{};

        sub["color"] = value.p2Config.color;
        sub["enabled"] = value.p2Config.enabled;

        obj["p2"] = sub;

        return obj;
    }

    static Result<WaveColor> fromJson(const matjson::Value& value) {
        return Ok(WaveColor{
            WaveColorConfig{
                ccc3(value["p1"]["color"]["r"].asInt().unwrapOr(255), value["p1"]["color"]["g"].asInt().unwrapOr(255), value["p1"]["color"]["b"].asInt().unwrapOr(255)),
                value["p1"]["enabled"].asBool().unwrapOr(false)
            },
            WaveColorConfig{
                ccc3(value["p2"]["color"]["r"].asInt().unwrapOr(255), value["p2"]["color"]["g"].asInt().unwrapOr(255), value["p2"]["color"]["b"].asInt().unwrapOr(255)),
                value["p2"]["enabled"].asBool().unwrapOr(false)
            }
        });
    }

};

class WaveColorSetting : public SettingBaseValueV3<WaveColor> {

public:

    static Result<std::shared_ptr<SettingV3>> parse(const std::string&, const std::string&, const matjson::Value&); 

    SettingNodeV3* createNode(float) override;

};

template <>
struct SettingTypeForValueType<WaveColor> {
    using SettingType = WaveColorSetting;
};

class WaveColorSettingSubNode : public BaseSettingSubNode<WaveColorConfig> {

private:

    Button* m_colorButton = nullptr;

    WaveColorSettingSubNode(std::string name)
        : BaseSettingSubNode(std::move(name)) {}

    bool init(float);

    void setValue(const WaveColorConfig&) override;
    void onReset() override;
    void setEnabled(bool) override;

public:

    static WaveColorSettingSubNode* create(float);

};