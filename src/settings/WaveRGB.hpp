#pragma once

#include "Includes.hpp"

#include "base/BaseSettingConfig.hpp"
#include "base/BaseSettingSubNode.hpp"

#include <Geode/loader/SettingV3.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/ui/Button.hpp>
#include <Geode/ui/SliderNode.hpp>

struct WaveRGBConfig {
    float speed = 1.f;
    float saturation = 1.f;
    bool enabled = false;
  
    bool operator==(const WaveRGBConfig&) const = default;
};

using WaveRGB = BaseSettingConfig<WaveRGBConfig, "separate-wave-rgb">;

template <>
struct matjson::Serialize<WaveRGB> {
    
    static matjson::Value toJson(const WaveRGB& value) {
        auto obj = matjson::Value{};
        auto sub = matjson::Value{};

        sub["speed"] = value.p1Config.speed;
        sub["saturation"] = value.p1Config.saturation;
        sub["enabled"] = value.p1Config.enabled;

        obj["p1"] = sub;

        sub = matjson::Value{};

        sub["speed"] = value.p2Config.speed;
        sub["saturation"] = value.p2Config.saturation;
        sub["enabled"] = value.p2Config.enabled;

        obj["p2"] = sub;

        return obj;
    }

    static Result<WaveRGB> fromJson(const matjson::Value& value) {
        return Ok(WaveRGB{
            WaveRGBConfig{
                static_cast<float>(value["p1"]["speed"].asDouble().unwrapOr(1.f)),
                static_cast<float>(value["p1"]["saturation"].asDouble().unwrapOr(1.f)),
                value["p1"]["enabled"].asBool().unwrapOr(false)
            },
            WaveRGBConfig{
                static_cast<float>(value["p2"]["speed"].asDouble().unwrapOr(1.f)),
                static_cast<float>(value["p2"]["saturation"].asDouble().unwrapOr(1.f)),
                value["p2"]["enabled"].asBool().unwrapOr(false)
            }
        });
    }

};

class WaveRGBSetting : public SettingBaseValueV3<WaveRGB> {

public:

    static Result<std::shared_ptr<SettingV3>> parse(const std::string&, const std::string&, const matjson::Value&); 

    SettingNodeV3* createNode(float) override;

};

template <>
struct SettingTypeForValueType<WaveRGB> {
    using SettingType = WaveRGBSetting;
};

class WaveRGBPreviewSettingNode : public BaseSettingSubNode<WaveRGBConfig> {

private:

    CCSprite* m_sprite = nullptr;
    float m_rgbTime = 0.f;

    WaveRGBPreviewSettingNode(std::string name)
        : BaseSettingSubNode(std::move(name)) {}

    bool init(float);

    void updateRGB(float);

    void setValue(const WaveRGBConfig&) override;
    void setEnabled(bool) override;

public:

    static WaveRGBPreviewSettingNode* create(float);

};

class WaveRGBSpeedSettingNode : public BaseSettingSubNode<WaveRGBConfig> {

private:

    TextInput* m_input = nullptr;
    SliderNode* m_slider = nullptr;
    CCLabelBMFont* m_label = nullptr;
    bool m_ignoreCallback = false;

    WaveRGBSpeedSettingNode(std::string name)
        : BaseSettingSubNode(std::move(name)) {}

    bool init(float);

    void setValue(const WaveRGBConfig&) override;
    void onReset() override;
    void setEnabled(bool) override;

public:

    static WaveRGBSpeedSettingNode* create(float);

};

class WaveRGBSaturationSettingNode : public BaseSettingSubNode<WaveRGBConfig> {

private:

    TextInput* m_input = nullptr;
    SliderNode* m_slider = nullptr;
    CCLabelBMFont* m_label = nullptr;
    bool m_ignoreCallback = false;

    WaveRGBSaturationSettingNode(std::string name)
        : BaseSettingSubNode(std::move(name)) {}

    bool init(float);

    void setValue(const WaveRGBConfig&) override;
    void onReset() override;
    void setEnabled(bool) override;

public:

    static WaveRGBSaturationSettingNode* create(float);

};