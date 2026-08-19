#include "WavePulseSize.hpp"

#include "base/BaseSettingNode.hpp"
#include "base/EnableSettingSubNode.hpp"

Result<std::shared_ptr<SettingV3>> WavePulseSizeSetting::parse(
    const std::string& key,
    const std::string& modID,
    const matjson::Value& json
) {
    auto res = std::make_shared<WavePulseSizeSetting>();
    auto root = checkJson(json, "WavePulseSizeSetting");
    
    res->parseBaseProperties(key, modID, root);
    root.checkUnknownKeys();

    return root.ok(std::static_pointer_cast<SettingV3>(res));
}

SettingNodeV3* WavePulseSizeSetting::createNode(float width) {
    auto ret = BaseSettingNode<
        "separate-wave-pulse-size",
        WavePulseSizeSetting,
        WavePulseSizeConfig
    >::create(
        std::static_pointer_cast<WavePulseSizeSetting>(shared_from_this()),
        width
    );

    std::array<std::vector<BaseSettingSubNode<WavePulseSizeConfig>*>, 2> nodes;

    for (int i = 0; i < 2; i++) {
        nodes[i] = {
            WavePulseSizeSettingNode::create(width),
            EnableSettingSubNode<"wave-pulse-size", WavePulseSize, WavePulseSizeConfig>::create(width, "Wave Pulse Size")
        };
    }

    ret->setNodes(std::move(nodes[0]), std::move(nodes[1]));

    return ret;
}

WavePulseSizeSettingNode* WavePulseSizeSettingNode::create(float width) {
    auto ret = new WavePulseSizeSettingNode("Pulse Size");

    if (ret->init(width)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}

bool WavePulseSizeSettingNode::init(float width) {
    auto container = CCNode::create();

    m_input = TextInput::create(60, "1.0");
    m_input->setScale(0.48f);

    container->addChild(m_input);

    container->addChild(AxisGap::create(2.5f));

    m_label = CCLabelBMFont::create("x", "bigFont.fnt");
    m_label->setOpacity(185);
    m_label->setScale(0.31f);

    container->addChild(m_label);

    container->addChild(AxisGap::create(11.f));

    m_slider = SliderNode::create([this](SliderNode* sender, float value) {
        if (m_ignoreCallback) {
            return;
        }
        
        m_ignoreCallback = true;

        m_config.size = value;
        m_callback(m_config);
       
        m_ignoreCallback = false;
    });
    m_slider->setMin(0.f);
    m_slider->setMax(10.f);
    m_slider->linkTextInput(m_input, 2);
    m_slider->setScale(0.45f);

    container->addChild(m_slider);

    BaseSettingSubNode::init(width, container);

    return true;
}

void WavePulseSizeSettingNode::setValue(const WavePulseSizeConfig& config) {
    BaseSettingSubNode<WavePulseSizeConfig>::setValue(config);

    if (!m_ignoreCallback) {
        m_ignoreCallback = true;

        m_slider->setValue(config.size);

        m_ignoreCallback = false;
    }

    m_resetSettingButton->setVisible(config.size != WavePulseSizeConfig().size);

    auto current = this->getMode() == SettingMode::P2 ? getSettingFast<"wave-pulse-size", WavePulseSize>().p2Config : getSettingFast<"wave-pulse-size", WavePulseSize>().p1Config; 

    m_nameLabel->setColor(current.size != config.size ? ccc3(17, 221, 0) : ccc3(255, 255, 255));

    this->setEnabled(current.enabled);
}

void WavePulseSizeSettingNode::onReset() {
    m_config.size = WavePulseSizeConfig().size;
    m_callback(m_config);
}

void WavePulseSizeSettingNode::setEnabled(bool enabled) {
    BaseSettingSubNode<WavePulseSizeConfig>::setEnabled(enabled);
    m_label->setOpacity(enabled ? 185 : 80);
    m_input->setEnabled(enabled);
    m_input->getBGSprite()->setOpacity(enabled ? 90 : 45);
    m_input->getInputNode()->getTextLabel()->setOpacity(enabled ? 255 : 110);
    m_slider->setReadOnly(!enabled);
    m_slider->setOpacity(enabled ? 255 : 90);
}