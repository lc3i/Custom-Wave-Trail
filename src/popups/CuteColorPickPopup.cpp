#include "CuteColorPickPopup.hpp"
#include "ColorCatalogPopup.hpp"

CuteColorPickPopup* CuteColorPickPopup::create(const ccColor3B& initialColor, Function<void(const ccColor3B&)> callback) {
    auto ret = new CuteColorPickPopup(std::move(callback));

    if (ret->init(initialColor)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}

bool CuteColorPickPopup::init(const ccColor3B& initialColor) {
    Popup::init(255, 227);

    this->setTitle("Select Color");

    auto container = CCNode::create();
    container->setAnchorPoint({0.5f, 0.5f});
    container->setPosition({m_size.width / 2.f + 3, 115});
    container->setLayout(
        SimpleAxisLayout::create(Axis::Row)
            ->setGap(10.f)
            ->setMainAxisScaling(AxisScaling::Grow)
            ->setCrossAxisScaling(AxisScaling::Grow)
    );

    m_mainLayer->addChild(container);

    m_colorButton = Button::createWithSpriteFrameName("GJ_colorBtn_001.png", [](Button*) {});
    m_colorButton->setScaleMultiplier(1.12f);

    container->addChild(m_colorButton);
    container->updateLayout();

    auto picker = CCControlColourPicker::colourPicker();
    picker->setPosition(picker->getContentSize() / 2.f);
    picker->setDelegate(this);

    auto subContainer = CCNode::create();
    subContainer->setScale(0.9f);
    subContainer->setContentSize(picker->getContentSize());

    subContainer->addChild(picker);

    m_colorButton->setActivateCallback([picker](Button*) {
        ColorCatalogPopup::create([picker](const ccColor3B& color) {
            picker->setColorValue(color);
        })->show();
    });

    container->addChild(subContainer);
    container->updateLayout();

    auto lbl = CCLabelBMFont::create("Presets", "goldFont.fnt");
    lbl->setScale(0.5f);
    lbl->setAnchorPoint({0, 0.5f});
    lbl->setPosition({16, 35});

    m_mainLayer->addChild(lbl);

    container = CCNode::create();
    container->setAnchorPoint({0, 0.5f});
    container->setPosition({16, 19});
    container->setLayout(
        SimpleAxisLayout::create(Axis::Row)
            ->setGap(3.f)
            ->setMainAxisScaling(AxisScaling::Grow)
            ->setCrossAxisScaling(AxisScaling::Grow)
            ->setMainAxisAlignment(MainAxisAlignment::Start)
    );

    m_mainLayer->addChild(container);

    auto btn = Button::createWithNode(
        ButtonSprite::create("Main Color", "goldFont.fnt", "GJ_button_04.png"),
        [picker](Button*) {
            picker->setColorValue(GameManager::get()->colorForIdx(GameManager::get()->getPlayerColor()));
        }
    );
    btn->setScaleMultiplier(1.05f);
    btn->setScale(0.31f);

    container->addChild(btn);

    btn = Button::createWithNode(
        ButtonSprite::create("Secondary Color", "goldFont.fnt", "GJ_button_04.png"),
        [picker](Button*) {
            picker->setColorValue(GameManager::get()->colorForIdx(GameManager::get()->getPlayerColor2()));
        }
    );
    btn->setScaleMultiplier(1.05f);
    btn->setScale(0.31f);
    
    container->addChild(btn);

    btn = Button::createWithNode(
        ButtonSprite::create("Glow Color", "goldFont.fnt", "GJ_button_04.png"),
        [picker](Button*) {
            picker->setColorValue(GameManager::get()->colorForIdx(GameManager::get()->getPlayerGlowColor()));
        }
    );
    btn->setScaleMultiplier(1.05f);
    btn->setScale(0.31f);
    
    container->addChild(btn);
    container->updateLayout();

    container = CCNode::create();

    m_input = TextInput::create(130, "...");
    m_input->setScale(0.45f);
    m_input->setPosition({45, 83});  
    m_input->setCallback([this, picker](const std::string& str) {
        m_ignore = true;

        picker->setColorValue(cc3bFromHexString(str, true).unwrapOr(ccColor3B{255, 255, 255}));

        m_ignore = false;
    });

    m_mainLayer->addChild(m_input);

    picker->setColorValue(initialColor);

    return true;
}

void CuteColorPickPopup::colorValueChanged(ccColor3B color) {
    m_colorButton->setColor(color);

    if (!m_ignore) {
        m_input->setString(cc3bToHexString(color));
    }
}

void CuteColorPickPopup::onClose(CCObject*) {
    m_callback(m_colorButton->getColor());
    Popup::onClose(nullptr);
}