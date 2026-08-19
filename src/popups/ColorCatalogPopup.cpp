#include "ColorCatalogPopup.hpp"

#include <Geode/ui/Button.hpp>

ColorCatalogPopup* ColorCatalogPopup::create(Function<void(const ccColor3B&)> onSelect) {
    auto ret = new ColorCatalogPopup(std::move(onSelect));

    if (ret->init()) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}

bool ColorCatalogPopup::init() {
    Popup::init(450, 245, "GJ_square05.png");
    
    this->setTitle("Select Color");

    const auto createButton = [this](int color, const CCPoint& pos) {
        auto btn = Button::createWithSpriteFrameName("GJ_colorBtn_001.png", [this](Button* sender) {
            m_onSelect(sender->getColor());
            this->onClose(nullptr);
        });
        btn->setColor(GameManager::get()->colorForIdx(color));
        btn->setScale(0.65f);
        btn->setPosition(pos + CCPoint{10, -23});

        m_mainLayer->addChild(btn);
    };

    createButton(51, CCPoint{17, 217});
    createButton(19, CCPoint{41, 217});
    createButton(48, CCPoint{65, 217});
    createButton(9, CCPoint{89, 217});
    createButton(62, CCPoint{125, 217});
    createButton(63, CCPoint{149, 217});
    createButton(10, CCPoint{173, 217});
    createButton(29, CCPoint{197, 217});
    createButton(70, CCPoint{233, 217});
    createButton(42, CCPoint{257, 217});
    createButton(11, CCPoint{281, 217});
    createButton(27, CCPoint{305, 217});
    createButton(72, CCPoint{341, 217});
    createButton(73, CCPoint{365, 217});
    createButton(0, CCPoint{389, 217});
    createButton(1, CCPoint{413, 217});
    createButton(37, CCPoint{17, 193});
    createButton(53, CCPoint{41, 193});
    createButton(54, CCPoint{65, 193});
    createButton(55, CCPoint{89, 193});
    createButton(26, CCPoint{125, 193});
    createButton(59, CCPoint{149, 193});
    createButton(60, CCPoint{173, 193});
    createButton(61, CCPoint{197, 193});
    createButton(71, CCPoint{233, 193});
    createButton(14, CCPoint{257, 193});
    createButton(31, CCPoint{281, 193});
    createButton(45, CCPoint{305, 193});
    createButton(105, CCPoint{341, 193});
    createButton(28, CCPoint{365, 193});
    createButton(32, CCPoint{389, 193});
    createButton(20, CCPoint{413, 193});
    createButton(25, CCPoint{17, 169});
    createButton(56, CCPoint{41, 169});
    createButton(57, CCPoint{65, 169});
    createButton(58, CCPoint{89, 169});
    createButton(30, CCPoint{125, 169});
    createButton(64, CCPoint{149, 169});
    createButton(65, CCPoint{173, 169});
    createButton(66, CCPoint{197, 169});
    createButton(46, CCPoint{233, 169});
    createButton(67, CCPoint{257, 169});
    createButton(68, CCPoint{281, 169});
    createButton(69, CCPoint{305, 169});
    createButton(2, CCPoint{341, 169});
    createButton(38, CCPoint{365, 169});
    createButton(79, CCPoint{389, 169});
    createButton(80, CCPoint{413, 169});
    createButton(74, CCPoint{17, 135.4f});
    createButton(75, CCPoint{41, 135.4f});
    createButton(44, CCPoint{65, 135.4f});
    createButton(3, CCPoint{89, 135.4f});
    createButton(83, CCPoint{125, 135.4f});
    createButton(16, CCPoint{149, 135.4f});
    createButton(4, CCPoint{173, 135.4f});
    createButton(5, CCPoint{197, 135.4f});
    createButton(52, CCPoint{233, 135.4f});
    createButton(41, CCPoint{257, 135.4f});
    createButton(6, CCPoint{281, 135.4f});
    createButton(35, CCPoint{305, 135.4f});
    createButton(98, CCPoint{341, 135.4f});
    createButton(8, CCPoint{365, 135.4f});
    createButton(36, CCPoint{389, 135.4f});
    createButton(103, CCPoint{413, 135.4f});
    createButton(40, CCPoint{17, 111.399994f});
    createButton(76, CCPoint{41, 111.399994f});
    createButton(77, CCPoint{65, 111.399994f});
    createButton(78, CCPoint{89, 111.399994f});
    createButton(22, CCPoint{125, 111.399994f});
    createButton(39, CCPoint{149, 111.399994f});
    createButton(84, CCPoint{173, 111.399994f});
    createButton(50, CCPoint{197, 111.399994f});
    createButton(47, CCPoint{233, 111.399994f});
    createButton(23, CCPoint{257, 111.399994f});
    createButton(92, CCPoint{281, 111.399994f});
    createButton(93, CCPoint{305, 111.399994f});
    createButton(7, CCPoint{341, 111.399994f});
    createButton(13, CCPoint{365, 111.399994f});
    createButton(24, CCPoint{389, 111.399994f});
    createButton(104, CCPoint{413, 111.399994f});
    createButton(33, CCPoint{17, 87.399994f});
    createButton(21, CCPoint{41, 87.399994f});
    createButton(81, CCPoint{65, 87.399994f});
    createButton(82, CCPoint{89, 87.399994f});
    createButton(34, CCPoint{125, 87.399994f});
    createButton(85, CCPoint{149, 87.399994f});
    createButton(86, CCPoint{173, 87.399994f});
    createButton(87, CCPoint{197, 87.399994f});
    createButton(49, CCPoint{233, 87.399994f});
    createButton(95, CCPoint{257, 87.399994f});
    createButton(96, CCPoint{281, 87.399994f});
    createButton(97, CCPoint{305, 87.399994f});
    createButton(43, CCPoint{341, 87.399994f});
    createButton(99, CCPoint{365, 87.399994f});
    createButton(100, CCPoint{389, 87.399994f});
    createButton(101, CCPoint{413, 87.399994f});
    createButton(106, CCPoint{125, 63.399994f});
    createButton(88, CCPoint{149, 63.399994f});
    createButton(89, CCPoint{173, 63.399994f});
    createButton(90, CCPoint{197, 63.399994f});
    createButton(12, CCPoint{269, 53.799988f});
    createButton(91, CCPoint{293, 53.799988f});
    createButton(17, CCPoint{317, 53.799988f});
    createButton(102, CCPoint{341, 53.799988f});
    createButton(18, CCPoint{365, 53.799988f});
    createButton(94, CCPoint{389, 53.799988f});
    createButton(15, CCPoint{413, 53.799988f});  

    return true;
}