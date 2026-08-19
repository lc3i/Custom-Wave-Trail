#include "WaveColor.hpp"

#include "base/BaseSettingNode.hpp"
#include "base/EnableSettingSubNode.hpp"
#include "popups/CuteColorPickPopup.hpp"

Result<std::shared_ptr<SettingV3>> WaveColorSetting::parse(
    const std::string& key,
    const std::string& modID,
    const matjson::Value& json
) {
    auto res = std::make_shared<WaveColorSetting>();
    auto root = checkJson(json, "WaveColorSetting");
    
    res->parseBaseProperties(key, modID, root);
    root.checkUnknownKeys();

    return root.ok(std::static_pointer_cast<SettingV3>(res));
}

SettingNodeV3* WaveColorSetting::createNode(float width) {
    auto ret = BaseSettingNode<
        "separate-wave-color",
        WaveColorSetting,
        WaveColorConfig
    >::create(
        std::static_pointer_cast<WaveColorSetting>(shared_from_this()),
        width
    );

    std::array<std::vector<BaseSettingSubNode<WaveColorConfig>*>, 2> nodes;

    for (int i = 0; i < 2; i++) {
        nodes[i] = {
            WaveColorSettingSubNode::create(width),
            EnableSettingSubNode<"wave-color", WaveColor, WaveColorConfig>::create(width, "Player Wave Color")
        };
    }

    ret->setNodes(std::move(nodes[0]), std::move(nodes[1]));

    return ret;
}

WaveColorSettingSubNode* WaveColorSettingSubNode::create(float width) {
    auto ret = new WaveColorSettingSubNode("Wave Trail Color");

    if (ret->init(width)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}

bool WaveColorSettingSubNode::init(float width) {
    auto container = CCNode::create();

    m_colorButton = Button::createWithSpriteFrameName("GJ_colorBtn_001.png", [this](Button*) {
        CuteColorPickPopup::create(m_colorButton->getColor(), [this](const ccColor3B& color) {
            m_config.color = color;
            m_callback(m_config);
        })->show();
    });
    m_colorButton->setScale(0.65f);
    
    container->addChild(m_colorButton);
    container->addChild(AxisGap::create(-1.725f));

    BaseSettingSubNode::init(width, container);

    return true;
}

void WaveColorSettingSubNode::setValue(const WaveColorConfig& config) {
    BaseSettingSubNode<WaveColorConfig>::setValue(config);

    m_colorButton->setColor(config.color);

    m_resetSettingButton->setVisible(config.color != WaveColorConfig().color);

    auto current = this->getMode() == SettingMode::P2 ? getSettingFast<"wave-color", WaveColor>().p2Config : getSettingFast<"wave-color", WaveColor>().p1Config; 

    m_nameLabel->setColor(current.color != config.color ? ccc3(17, 221, 0) : ccc3(255, 255, 255));

    this->setEnabled(current.enabled);
}

void WaveColorSettingSubNode::onReset() {
    m_config.color = WaveColorConfig().color;
    m_callback(m_config);
}

void WaveColorSettingSubNode::setEnabled(bool enabled) {
    BaseSettingSubNode<WaveColorConfig>::setEnabled(enabled);
    m_colorButton->setOpacity(enabled ? 255 : 100);
    m_colorButton->setColor(enabled ? m_config.color : ccc3(m_config.color.r * 0.5f, m_config.color.g * 0.5f, m_config.color.b * 0.5f));
    m_colorButton->setEnabled(enabled);
}