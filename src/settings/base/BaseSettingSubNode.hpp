#pragma once

#include "Includes.hpp"

#include <Geode/ui/Button.hpp>

template <typename T>
using SettingCallback = Function<void(const T&)>;

enum class SettingMode {
    Single,
    P1,
    P2
};

template<typename Config>
class BaseSettingSubNode : public CCNode {

private:

    float m_width;
    bool m_enabled = true;

protected:

    CCNode* m_subContainer = nullptr;
    Button* m_resetSettingButton = nullptr;
    CCLabelBMFont* m_nameLabel = nullptr;
    SettingCallback<Config> m_callback;
    SettingMode m_mode = SettingMode::Single;
    Config m_config;
    std::string m_name;

    BaseSettingSubNode(std::string name)
        : m_name(std::move(name)) {}

    void init(float width) {
        m_width = width;

        this->setContentSize({width, 30.f});

        m_subContainer = CCNode::create();
        m_subContainer->setAnchorPoint({0, 0.5f});
        m_subContainer->setPosition({10, 15});
        m_subContainer->setLayout(
            SimpleAxisLayout::create(Axis::Row)
                ->setGap(5.f)
                ->setMainAxisScaling(AxisScaling::Grow)
                ->setCrossAxisScaling(AxisScaling::Grow)
                ->setMainAxisAlignment(MainAxisAlignment::Start)
                ->ignoreInvisibleChildren(false)
        );

        this->addChild(m_subContainer);

        m_nameLabel = CCLabelBMFont::create(m_name.c_str(), "bigFont.fnt");
        m_nameLabel->limitLabelWidth(200.f, 0.4f, 0.f);

        m_subContainer->addChild(m_nameLabel);

        m_resetSettingButton = Button::createWithSpriteFrameName("geode.loader/reset-gold.png", [this](Button*) {
            createQuickPopup(
                "Reset",
                fmt::format(
                    "Are you sure you want to <cr>reset</c> <cl>{}</c> to <cy>default</c>?",
                    m_nameLabel->getString()
                ),
                "Cancel", "Reset",
                [this](auto, bool btn2) {
                    if (btn2) {
                        this->onReset();
                    }
                }
            );
        });
        m_resetSettingButton->setScale(0.5f);
        
        m_subContainer->addChild(m_resetSettingButton);
        m_subContainer->updateLayout();
    }

    void init(float width, CCNode* node) {
        BaseSettingSubNode::init(width);
        
        node->setLayout(
            SimpleAxisLayout::create(Axis::Row)
                ->setMainAxisScaling(AxisScaling::Grow)
                ->setCrossAxisScaling(AxisScaling::Grow)
                ->setMainAxisAlignment(MainAxisAlignment::Start)
        );
        node->updateLayout();
        node->setAnchorPoint({1, 0.5f});
        node->setPosition({width - 10, 15});

        this->addChild(node);
    }

    SettingMode getMode() {
        return m_mode;
    }

    bool isEnabled() {
        return m_enabled;
    }

    virtual void setEnabled(bool enabled) {
        m_enabled = enabled;
        m_nameLabel->setOpacity(enabled ? 255 : 100);
        m_resetSettingButton->setOpacity(enabled ? 255 : 100);
        m_resetSettingButton->setEnabled(enabled);
    }

    virtual void onReset() {}
    
public:

    virtual void setValue(const Config& config) {        
        m_config = config;
    }

    virtual void setMode(SettingMode mode) {
        if (mode == SettingMode::Single) {
            m_nameLabel->setString(m_name.c_str());
        } else {
            m_nameLabel->setString(fmt::format("{} {}", mode == SettingMode::P1 ? "P1" : "P2" , m_name).c_str());
        }

        m_subContainer->updateLayout();
    
        m_mode = mode;
    }

    void setCallback(SettingCallback<Config> callback) {
        m_callback = std::move(callback);
    }

};