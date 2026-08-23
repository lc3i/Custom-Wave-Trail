#include "PlayLayer.hpp"
#include "PlayerObject.hpp"

void ProPlayLayer::applyPlayerSettings(bool isReset) {
    static_cast<ProPlayerObject*>(m_player1)->applyAllSettings(isReset);
    static_cast<ProPlayerObject*>(m_player2)->applyAllSettings(isReset);
}

void ProPlayLayer::setupHasCompleted() {
    PlayLayer::setupHasCompleted();
    this->applyPlayerSettings();
}

void ProPlayLayer::resetLevel() {
    PlayLayer::resetLevel();
    this->applyPlayerSettings(true);
}

void ProPlayLayer::destroyPlayer(PlayerObject* p0, GameObject* p1) {
    PlayLayer::destroyPlayer(p0, p1);
    this->applyPlayerSettings();
}