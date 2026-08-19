#include "WaveRGB.hpp"

#include "base/BaseSettingNode.hpp"
#include "base/EnableSettingSubNode.hpp"

Result<std::shared_ptr<SettingV3>> WaveRGBSetting::parse(
    const std::string& key,
    const std::string& modID,
    const matjson::Value& json
) {
    auto res = std::make_shared<WaveRGBSetting>();
    auto root = checkJson(json, "WaveRGBSetting");
    
    res->parseBaseProperties(key, modID, root);
    root.checkUnknownKeys();

    return root.ok(std::static_pointer_cast<SettingV3>(res));
}

SettingNodeV3* WaveRGBSetting::createNode(float width) {
    auto ret = BaseSettingNode<
        "separate-wave-rgb",
        WaveRGBSetting,
        WaveRGBConfig
    >::create(
        std::static_pointer_cast<WaveRGBSetting>(shared_from_this()),
        width
    );

    std::array<std::vector<BaseSettingSubNode<WaveRGBConfig>*>, 2> nodes;

    for (int i = 0; i < 2; i++) {
        nodes[i] = {
            WaveRGBPreviewSettingNode::create(width),
            WaveRGBSpeedSettingNode::create(width),
            WaveRGBSaturationSettingNode::create(width),
            EnableSettingSubNode<"wave-rgb", WaveRGB, WaveRGBConfig>::create(width, "Wave Trail RGB")
        };
    }

    ret->setNodes(std::move(nodes[0]), std::move(nodes[1]));

    return ret;
}

WaveRGBPreviewSettingNode* WaveRGBPreviewSettingNode::create(float width) {
    auto ret = new WaveRGBPreviewSettingNode("RGB Preview");

    if (ret->init(width)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}

bool WaveRGBPreviewSettingNode::init(float width) {
    auto container = CCNode::create();

    m_sprite = CCSprite::createWithSpriteFrameName("GJ_colorBtn_001.png");
    m_sprite->setScale(0.65f);

    container->addChild(m_sprite);
    container->addChild(AxisGap::create(-1.725f));

    BaseSettingSubNode::init(width, container);

    return true;
}

void WaveRGBPreviewSettingNode::updateRGB(float dt) {
    auto duration = 5.f / m_config.speed;

    m_rgbTime += dt;

    if (m_rgbTime > duration) {
        m_rgbTime = m_rgbTime - duration;
    }

    auto t = m_rgbTime / duration;

    auto r = clampf(std::abs(6.f * t - 3.f) - 1.f, 0.f, 1.f);
    auto g = clampf(2.f - std::abs(6.f * t - 2.f), 0.f, 1.f);
    auto b = clampf(2.f - std::abs(6.f * t - 4.f), 0.f, 1.f);
    auto pastel = 1.f - m_config.saturation;

    r = r + (1.f - r) * pastel;
    g = g + (1.f - g) * pastel;
    b = b + (1.f - b) * pastel;

    auto color = ccc3(
        r * 255 * (this->isEnabled() ? 1.f : 0.5f),
        g * 255 * (this->isEnabled() ? 1.f : 0.5f),
        b * 255 * (this->isEnabled() ? 1.f : 0.5f)
    );

    m_sprite->setColor(color);
}

void WaveRGBPreviewSettingNode::setValue(const WaveRGBConfig& config) {
    BaseSettingSubNode<WaveRGBConfig>::setValue(config);

    m_resetSettingButton->setVisible(false);

    this->schedule(schedule_selector(WaveRGBPreviewSettingNode::updateRGB));

    auto current = this->getMode() == SettingMode::P2 ? getSettingFast<"wave-rgb", WaveRGB>().p2Config : getSettingFast<"wave-rgb", WaveRGB>().p1Config; 
    this->setEnabled(current.enabled);
}

void WaveRGBPreviewSettingNode::setEnabled(bool enabled) {
    BaseSettingSubNode<WaveRGBConfig>::setEnabled(enabled);
    m_sprite->setOpacity(enabled ? 255 : 100);
}

WaveRGBSpeedSettingNode* WaveRGBSpeedSettingNode::create(float width) {
    auto ret = new WaveRGBSpeedSettingNode("RGB Speed");

    if (ret->init(width)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}

bool WaveRGBSpeedSettingNode::init(float width) {
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

        m_config.speed = value;
        m_callback(m_config);
       
        m_ignoreCallback = false;
    });
    m_slider->setMin(0.05f);
    m_slider->setMax(5.f);
    m_slider->linkTextInput(m_input, 2);
    m_slider->setScale(0.45f);

    container->addChild(m_slider);

    BaseSettingSubNode::init(width, container);

    return true;
}

void WaveRGBSpeedSettingNode::setValue(const WaveRGBConfig& config) {
    BaseSettingSubNode<WaveRGBConfig>::setValue(config);

    if (!m_ignoreCallback) {
        m_ignoreCallback = true;

        m_slider->setValue(config.speed);

        m_ignoreCallback = false;
    }

    m_resetSettingButton->setVisible(config.speed != WaveRGBConfig().speed);

    auto current = this->getMode() == SettingMode::P2 ? getSettingFast<"wave-rgb", WaveRGB>().p2Config : getSettingFast<"wave-rgb", WaveRGB>().p1Config; 

    m_nameLabel->setColor(current.speed != config.speed ? ccc3(17, 221, 0) : ccc3(255, 255, 255));

    this->setEnabled(current.enabled);
}

void WaveRGBSpeedSettingNode::onReset() {
    m_config.speed = WaveRGBConfig().speed;
    m_callback(m_config);
}

void WaveRGBSpeedSettingNode::setEnabled(bool enabled) {
    BaseSettingSubNode<WaveRGBConfig>::setEnabled(enabled);
    m_label->setOpacity(enabled ? 185 : 80);
    m_input->setEnabled(enabled);
    m_input->getBGSprite()->setOpacity(enabled ? 90 : 45);
    m_input->getInputNode()->getTextLabel()->setOpacity(enabled ? 255 : 110);
    m_slider->setReadOnly(!enabled);
    m_slider->setOpacity(enabled ? 255 : 90);
}

WaveRGBSaturationSettingNode* WaveRGBSaturationSettingNode::create(float width) {
    auto ret = new WaveRGBSaturationSettingNode("RGB Saturation");

    if (ret->init(width)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}

bool WaveRGBSaturationSettingNode::init(float width) {
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

        m_config.saturation = value;
        m_callback(m_config);
       
        m_ignoreCallback = false;
    });
    m_slider->setMin(0.f);
    m_slider->setMax(1.f);
    m_slider->linkTextInput(m_input, 2);
    m_slider->setScale(0.45f);

    container->addChild(m_slider);

    BaseSettingSubNode::init(width, container);

    return true;
}

void WaveRGBSaturationSettingNode::setValue(const WaveRGBConfig& config) {
    BaseSettingSubNode<WaveRGBConfig>::setValue(config);

    if (!m_ignoreCallback) {
        m_ignoreCallback = true;

        m_slider->setValue(config.saturation);

        m_ignoreCallback = false;
    }

    m_resetSettingButton->setVisible(config.saturation != WaveRGBConfig().saturation);

    auto current = this->getMode() == SettingMode::P2 ? getSettingFast<"wave-rgb", WaveRGB>().p2Config : getSettingFast<"wave-rgb", WaveRGB>().p1Config; 

    m_nameLabel->setColor(current.saturation != config.saturation ? ccc3(17, 221, 0) : ccc3(255, 255, 255));

    this->setEnabled(current.enabled);
}

void WaveRGBSaturationSettingNode::onReset() {
    m_config.saturation = WaveRGBConfig().saturation;
    m_callback(m_config);
}

void WaveRGBSaturationSettingNode::setEnabled(bool enabled) {
    BaseSettingSubNode<WaveRGBConfig>::setEnabled(enabled);
    m_label->setOpacity(enabled ? 185 : 80);
    m_input->setEnabled(enabled);
    m_input->getBGSprite()->setOpacity(enabled ? 90 : 45);
    m_input->getInputNode()->getTextLabel()->setOpacity(enabled ? 255 : 110);
    m_slider->setReadOnly(!enabled);
    m_slider->setOpacity(enabled ? 255 : 90);
}