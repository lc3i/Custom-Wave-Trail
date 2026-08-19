#include "PauseLayer.hpp"

#include "settings/WaveRGB.hpp"
#include "settings/WaveTrailSize.hpp"
#include "settings/NoWavePulse.hpp"
#include "settings/WavePulseSize.hpp"
#include "settings/SolidWaveTrail.hpp"
#include "settings/NoWaveTrail.hpp"

#include <Geode/ui/Button.hpp>

void ProPauseLayer::applyTrailSettings() {
    auto f = m_fields.self();
    auto pl = PlayLayer::get();

    if (!pl || !f->backTrail) {
        return;
    }

    f->backTrail->setColor(pl->m_player1->m_waveTrail->getColor());
    f->backTrail->setContentWidth(getSettingFast<"wave-trail-size", WaveTrailSize>().get(true).size * 3.5f);
    f->frontTrail->setContentWidth(getSettingFast<"wave-trail-size", WaveTrailSize>().get(true).size * 1.47f);
    f->frontTrail->setVisible(!getSettingFast<"solid-wave-trail", SolidWaveTrail>().get(true) && !getSettingFast<"no-wave-trail", NoWaveTrail>());
    f->backTrail->setBlendFunc(getSettingFast<"solid-wave-trail", SolidWaveTrail>().get(true) ? ccBlendFunc{GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA} : ccBlendFunc{GL_SRC_ALPHA, GL_ONE});
    f->backTrail->setVisible(!getSettingFast<"no-wave-trail", NoWaveTrail>());
}

void ProPauseLayer::updateTrail(float dt) {
    auto pl = PlayLayer::get();

    if (!pl) {
        return;
    }

    auto f = m_fields.self();

    if (!f->backTrail) {
        this->unschedule(schedule_selector(ProPauseLayer::updateTrail));
        return;
    }

    if (!getSettingFast<"no-wave-pulse", NoWavePulse>().get(true)) {
        f->pulseTime += dt;
        
        auto duration = 0.4f;
    
        if (f->pulseTime > duration) {
            f->pulseTime -= duration;
        }

        auto p = f->pulseTime / duration;
        float t;

        if (p < 0.3f) {
            float x = p / 0.3f;
            t = x;
        } else {
            float x = (p - 0.3f) / 0.7f;
            t = 1.f - x;
        }

        float riseSize = 0.5f;
        float downSize = 0.35f;

        auto scale = (1.f - downSize) + t * (riseSize + downSize);
        
        const auto& pulseConfig = getSettingFast<"wave-pulse-size", WavePulseSize>().get(true);

        if (pulseConfig.enabled) {
            auto size = std::max(pulseConfig.size / 4.f, 1.f);
            if (scale > 1.f) {
                scale = pow(scale, size * size) * size;
            }
        }

        f->backTrail->setScaleX(scale);
        f->frontTrail->setScaleX(scale);
    } else {
        f->backTrail->setScaleX(1.f);
        f->frontTrail->setScaleX(1.f);
    }

    const auto& rgbConfig = getSettingFast<"wave-rgb", WaveRGB>().get(true);
    
    if (!rgbConfig.enabled) {
        return;
    }

    auto duration = 5.f / rgbConfig.speed;

    f->rgbTime += dt;

    if (f->rgbTime > duration) {
        f->rgbTime = f->rgbTime - duration;
    }

    auto t = f->rgbTime / duration;

    auto r = clampf(std::abs(6.f * t - 3.f) - 1.f, 0.f, 1.f);
    auto g = clampf(2.f - std::abs(6.f * t - 2.f), 0.f, 1.f);
    auto b = clampf(2.f - std::abs(6.f * t - 4.f), 0.f, 1.f);
    auto pastel = 1.f - rgbConfig.saturation;

    r = r + (1.f - r) * pastel;
    g = g + (1.f - g) * pastel;
    b = b + (1.f - b) * pastel;

    auto color = ccc3(
        r * 255,
        g * 255,
        b * 255
    );

    f->backTrail->setColor(color);
}

void ProPauseLayer::customSetup() {
    PauseLayer::customSetup();

    if (!getSettingFast<"button-in-pause-menu", bool>()) {
        return;
    }

    auto menu = this->getChildByID("left-button-menu");

    if (!menu) {
        return;
    }

    auto spr = CircleButtonSprite::create(CCNode::create(), CircleBaseColor::Gray, CircleBaseSize::Medium);
    spr->setScale(0.7f);
    
    auto btn = Button::createWithNode(spr, [](Button*) {
        toggleSettingsPopup();
    });
    btn->setID("button"_spr);

    menu->addChild(btn);
    menu->updateLayout();

    auto stencil = CCSprite::createWithSpriteFrameName("d_circle_02_001.png");
    stencil->setScale(0.517f);
    stencil->setPosition(btn->getContentSize() / 2.f);

    auto clip = CCClippingNode::create();
    clip->setAlphaThreshold(0.01f);
    clip->setStencil(stencil);

    btn->addChild(clip);

    auto gm = GameManager::get();

    auto icon = SimplePlayer::create(gm->getPlayerFrame());
    icon->updatePlayerFrame(gm->getPlayerDart(), IconType::Wave);
    icon->setRotation(-35);
    icon->setPosition({27.05f, 24.625f});
    icon->setScale(0.5f);
    icon->setColors(
        gm->colorForIdx(gm->getPlayerColor()),
        gm->colorForIdx(gm->getPlayerColor2())
    );
    if (gm->getPlayerGlow()) {
        icon->setGlowOutline(gm->colorForIdx(gm->getPlayerGlowColor()));
        icon->enableCustomGlowColor(gm->colorForIdx(gm->getPlayerGlowColor()));
    }

    clip->addChild(icon, 2);

    auto f = m_fields.self();

    auto layer = CCLayerColor::create({ 0, 0, 0, 180 }, 40, 40);

    clip->addChild(layer);

    layer = CCLayerColor::create({ 255, 0, 0, 255 }, 3.5f, 40);
    layer->ignoreAnchorPointForPosition(false);
    layer->setRotation(55);
    layer->setPosition({12.5f, 14.4f});
    layer->setBlendFunc({GL_SRC_ALPHA, GL_ONE});

    clip->addChild(layer);

    f->backTrail = layer;

    layer = CCLayerColor::create({ 255, 255, 255, 172 }, 1.47f, 40);
    layer->ignoreAnchorPointForPosition(false);
    layer->setRotation(55);
    layer->setPosition({12.5f, 14.4f});
    layer->setBlendFunc({GL_SRC_ALPHA, GL_ONE});

    f->frontTrail = layer;

    clip->addChild(layer);

    this->applyTrailSettings();
    this->schedule(schedule_selector(ProPauseLayer::updateTrail));
}