#pragma once

#include "Includes.hpp"

#include "BaseSettingSubNode.hpp"

template<string::ConstexprString SettingId, typename Struct, typename Config>
class EnableSettingSubNode : public BaseSettingSubNode<Config> {

private:

    using Super = BaseSettingSubNode<Config>;

    CCMenuItemToggler* m_toggle = nullptr;

    EnableSettingSubNode(std::string name)
        : Super(std::move(name)) {}

    void init(float width) {
        auto container = CCMenu::create();
        container->setContentSize({0, 0});

        m_toggle = CCMenuItemExt::createTogglerWithStandardSprites(0.55f, [this](CCMenuItemToggler*) {
            queueInMainThread([self = Ref(this)] {
                self->Super::m_config.enabled = self->m_toggle->isToggled();
                self->Super::m_callback(self->Super::m_config);
            });
        });
        m_toggle->setCascadeOpacityEnabled(true);

        container->addChild(m_toggle);
        container->addChild(AxisGap::create(1.725f));

        Super::init(width, container);

        Super::m_resetSettingButton->setActivateCallback([this](Button*) {
            createQuickPopup(
                "Reset",
                fmt::format(
                    "Are you sure you want to <cr>reset</c> <cl>{} {}</c> to <cy>default</c>?",
                    Super::m_nameLabel->getString(), Super::m_name
                ),
                "Cancel", "Reset",
                [this](auto, bool btn2) {
                    if (btn2) {
                        this->onReset();
                    }
                }
            );
        });
    }

    void setValue(const Config& config) override {
        Super::setValue(config);
        
        m_toggle->toggle(config.enabled);
        
        Super::m_resetSettingButton->setVisible(config.enabled != Config().enabled);

        auto current = this->getMode() == SettingMode::P2 ? getSettingFast<SettingId, Struct>().p2Config : getSettingFast<SettingId, Struct>().p1Config; 

        Super::m_nameLabel->setColor(current.enabled != config.enabled ? ccc3(17, 221, 0) : ccc3(255, 255, 255));
    }

    void setMode(SettingMode mode) override {
        if (mode == SettingMode::Single) {
            Super::m_nameLabel->setString("Enable");
        } else {
            Super::m_nameLabel->setString(fmt::format("Enable {}", mode == SettingMode::P1 ? "P1" : "P2").c_str());
        }

        Super::m_subContainer->updateLayout();
    
        Super::m_mode = mode;
    }

    void onReset() override {
        Super::m_config.enabled = Config().enabled;
        Super::m_callback(Super::m_config);
    }

    // void setEnabled(bool enabled) override {
    //     Super::setEnabled(enabled);
    //     m_toggle->setOpacity(enabled ? 255 : 100);
    //     m_toggle->setEnabled(enabled);
    // }

public:

    static EnableSettingSubNode<SettingId, Struct, Config>* create(float width, std::string name) {
        auto ret = new EnableSettingSubNode<SettingId, Struct, Config>(std::move(name));
        ret->init(width);
        return ret;
    }

};