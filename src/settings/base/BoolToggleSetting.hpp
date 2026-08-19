#pragma once

#include "Includes.hpp"
#include "BaseSettingNode.hpp"
#include "BaseSettingSubNode.hpp"

#include <Geode/loader/SettingV3.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/ui/Button.hpp>

template <typename Tag, string::ConstexprString SeparateId>
struct BoolToggleValue {
    bool p1Config = false;
    bool p2Config = false;

    bool operator==(const BoolToggleValue&) const = default;

    explicit operator bool() const {
        return p1Config || p2Config;
    }

    const bool& get(bool player1) const {
        if (player1 || !getSettingFast<SeparateId, bool>()) {
            return p1Config;
        }
        return p2Config;
    }
};

template <typename Tag, string::ConstexprString SeparateId>
struct matjson::Serialize<BoolToggleValue<Tag, SeparateId>> {

    static matjson::Value toJson(const BoolToggleValue<Tag, SeparateId>& value) {
        auto obj = matjson::Value{};

        obj["p1"] = value.p1Config;
        obj["p2"] = value.p2Config;

        return obj;
    }

    static Result<BoolToggleValue<Tag, SeparateId>> fromJson(const matjson::Value& value) {
        return Ok(BoolToggleValue<Tag, SeparateId>{
            value["p1"].asBool().unwrapOr(false),
            value["p2"].asBool().unwrapOr(false)
        });
    }

};

template <string::ConstexprString SettingId, string::ConstexprString Name, typename Value>
class BoolToggleSubNode : public BaseSettingSubNode<bool> {

private:

    CCMenuItemToggler* m_toggle = nullptr;

    BoolToggleSubNode()
        : BaseSettingSubNode(std::string(Name.data())) {}

    bool init(float width) {
        auto container = CCMenu::create();
        container->setContentSize({0, 0});

        m_toggle = CCMenuItemExt::createTogglerWithStandardSprites(0.55f, [this](CCMenuItemToggler*) {
            queueInMainThread([self = Ref(this)] {
                self->m_config = self->m_toggle->isToggled();
                self->m_callback(self->m_config);
            });
        });
        m_toggle->setCascadeOpacityEnabled(true);

        container->addChild(m_toggle);
        container->addChild(AxisGap::create(1.725f));

        BaseSettingSubNode::init(width, container);

        return true;
    }

    void setValue(const bool& config) override {
        BaseSettingSubNode<bool>::setValue(config);

        m_toggle->toggle(config);

        this->m_resetSettingButton->setVisible(config != Value().p1Config);

        auto current = this->getMode() == SettingMode::P2 ? getSettingFast<SettingId, Value>().p2Config : getSettingFast<SettingId, Value>().p1Config;

        this->m_nameLabel->setColor(current != config ? ccc3(17, 221, 0) : ccc3(255, 255, 255));
    }

    void onReset() override {
        this->m_config = Value().p1Config;
        this->m_callback(this->m_config);
    }

public:

    static BoolToggleSubNode* create(float width) {
        auto ret = new BoolToggleSubNode();

        if (ret->init(width)) {
            ret->autorelease();
            return ret;
        }

        delete ret;
        return nullptr;
    }

};

template <string::ConstexprString SettingId, string::ConstexprString SeparateId, string::ConstexprString Name, typename Value>
class BoolToggleSetting : public SettingBaseValueV3<Value> {

public:

    static Result<std::shared_ptr<SettingV3>> parse(const std::string& key, const std::string& modID, const matjson::Value& json) {
        auto res = std::make_shared<BoolToggleSetting>();
        auto root = checkJson(json, "BoolToggleSetting");

        res->parseBaseProperties(key, modID, root);
        root.checkUnknownKeys();

        return root.ok(std::static_pointer_cast<SettingV3>(res));
    }

    SettingNodeV3* createNode(float width) override {
        auto ret = BaseSettingNode<
            SeparateId,
            BoolToggleSetting,
            bool
        >::create(
            std::static_pointer_cast<BoolToggleSetting>(this->shared_from_this()),
            width
        );

        std::array<std::vector<BaseSettingSubNode<bool>*>, 2> nodes;

        for (int i = 0; i < 2; i++) {
            nodes[i] = {
                BoolToggleSubNode<SettingId, Name, Value>::create(width)
            };
        }

        ret->setNodes(std::move(nodes[0]), std::move(nodes[1]));

        return ret;
    }

};
