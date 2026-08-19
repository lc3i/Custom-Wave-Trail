#pragma once

#include "Includes.hpp"
#include "BaseSettingSubNode.hpp"

template <string::ConstexprString SeparateId, typename Setting, typename Config>
class BaseSettingNode : public SettingValueNodeV3<Setting> {

private:

    CCNode* m_container = nullptr;
    std::vector<BaseSettingSubNode<Config>*> m_p1Nodes;
    std::vector<BaseSettingSubNode<Config>*> m_p2Nodes;

    bool init(std::shared_ptr<Setting> setting, float width) {
        if (!SettingValueNodeV3<Setting>::init(setting, width)) {
            return false;
        }

        m_container = CCNode::create();
        m_container->setContentWidth(width);
        m_container->setLayout(
            SimpleAxisLayout::create(Axis::Column)
                ->setMainAxisScaling(AxisScaling::Grow)
                ->setCrossAxisScaling(AxisScaling::None)
        );

        this->addChild(m_container);

        return true;
    }

    void updateState(CCNode* invoker) override {
        SettingValueNodeV3<Setting>::updateState(invoker);

        if (m_p1Nodes.empty() || m_p2Nodes.empty()) {
            return;
        }

        const auto& value = this->getValue();

        if (getSettingFast<SeparateId, bool>()) {
            for (auto node : m_p1Nodes) {
                node->setMode(SettingMode::P1);
            }

            for (auto node : m_p2Nodes) {
                node->setVisible(true);
                node->setMode(SettingMode::P2);
            }
        } else {
            for (auto node : m_p1Nodes) {
                node->setMode(SettingMode::Single);
            }

            for (auto node : m_p2Nodes) {
                node->setVisible(false);
            }
        }

        m_container->updateLayout();

        this->getNameMenu()->getChildrenExt().back()->setVisible(false);
        this->setContentHeight(m_container->getContentHeight());

        for (auto node : m_p1Nodes) {
            node->setValue(this->getValue().p1Config);
        }

        for (auto node : m_p2Nodes) {
            node->setValue(this->getValue().p2Config);
        }
    }

public:
    
    static BaseSettingNode* create(std::shared_ptr<Setting> setting, float width) {
        auto ret = new BaseSettingNode<SeparateId, Setting, Config>();

        if (ret->init(setting, width)) {
            ret->autorelease();
            return ret;
        }

        delete ret;
        return nullptr;
    }

    void setNodes(std::vector<BaseSettingSubNode<Config>*> p1Nodes, std::vector<BaseSettingSubNode<Config>*> p2Nodes) {
        m_p1Nodes = std::move(p1Nodes);
        m_p2Nodes = std::move(p2Nodes);

        for (auto node : m_p1Nodes) {
            m_container->addChild(node);
            node->setCallback([self = Ref(this)](const Config& config) { auto current = self->getValue(); current.p1Config = config; self->setValue(current, nullptr); });
        }

        for (auto node : m_p2Nodes) {
            m_container->addChild(node);
            node->setCallback([self = Ref(this)](const Config& config) { auto current = self->getValue(); current.p2Config = config; self->setValue(current, nullptr); });
        }

        this->updateState(nullptr);
    }

};