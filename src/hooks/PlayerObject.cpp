#include "PlayerObject.hpp"

#include "settings/WaveColor.hpp"
#include "settings/WaveRGB.hpp"
#include "settings/SolidWaveTrail.hpp"
#include "settings/WaveTrailSize.hpp"
#include "settings/WavePulseSize.hpp"
#include "settings/NoWaveTrail.hpp"
#include "settings/NoWavePulse.hpp"
#include "settings/KeepTrailOnDeath.hpp"
#include "settings/NoTrailBehind.hpp"

void ProPlayerObject::applyAllSettings(bool isReset) {
    if (!this->isVanillaPlayer()) {
        return;
    }

    if (isReset && this->isPlayer2()) {
        m_fields->didScheduleRGB = false;
    }

    this->applyTrailColor();
    this->applyTrailRGB();
    this->applyNoWaveTrail();
    this->applySolidWaveTrail();
    this->applyWaveTrailSize();
    this->applyWavePulseSize();
    this->applyNoTrailBehind();
}

void ProPlayerObject::applyTrailColor() {
    if (!this->isVanillaPlayer()) {
        return;
    }

    if (getSettingFast<"wave-rgb", WaveRGB>().get(this->isPlayer1()).enabled) {
        return;
    }
 
    const auto& config = getSettingFast<"wave-color", WaveColor>().get(this->isPlayer1());
    auto f = m_fields.self();

    if (!config.enabled) {
        if (f->didSetColor) {
            m_waveTrail->setColor(f->originalColor);
        }

        return;
    }

    if (!f->didSetColor) {
        f->didSetColor = true;
        f->originalColor = m_waveTrail->getColor();
    }

    m_waveTrail->setColor(config.color);
}

void ProPlayerObject::applyTrailRGB() {
    if (!this->isVanillaPlayer()) {
        return;
    }

    const auto& config = getSettingFast<"wave-rgb", WaveRGB>().get(this->isPlayer1());
    auto f = m_fields.self();

    if (!config.enabled) {
        if (f->didScheduleRGB) {
            this->unschedule(schedule_selector(ProPlayerObject::updateTrailRGB));
            f->didScheduleRGB = false;
        }

        return;
    }

    if (!f->didScheduleRGB) {
        f->didScheduleRGB = true;
        this->schedule(schedule_selector(ProPlayerObject::updateTrailRGB));
    }
}

void ProPlayerObject::applyNoWaveTrail() {
    if (!this->isVanillaPlayer()) {
        return;
    }

    m_waveTrail->setVisible(
        !getSettingFast<"no-wave-trail", NoWaveTrail>().get(this->isPlayer1())
    );
}

void ProPlayerObject::applyNoWavePulse() {
    if (!this->isVanillaPlayer()) {
        return;
    }
    
    if (getSettingFast<"no-wave-pulse", NoWavePulse>().get(this->isPlayer1())) {
        m_waveTrail->m_pulseSize = 1.4f;
    }
}

void ProPlayerObject::applySolidWaveTrail() {
    if (!this->isVanillaPlayer()) {
        return;
    }

    auto enabled = getSettingFast<"solid-wave-trail", SolidWaveTrail>().get(this->isPlayer1());

    m_waveTrail->setBlendFunc(enabled ? ccBlendFunc{GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA} : ccBlendFunc{GL_SRC_ALPHA, GL_ONE});
    m_waveTrail->m_isSolid = enabled;
}

void ProPlayerObject::applyWavePulseSize() {
    if (!this->isVanillaPlayer()) {
        return;
    }

    const auto& config = getSettingFast<"wave-pulse-size", WavePulseSize>().get(this->isPlayer1());

    if (!config.enabled) {
        return;
    }

    if (m_waveTrail->m_pulseSize > 1.24f) {
        m_waveTrail->m_pulseSize = pow(m_waveTrail->m_pulseSize, config.size * config.size) * config.size;
    }
}

void ProPlayerObject::applyWaveTrailSize() {
    if (!this->isVanillaPlayer()) {
        return;
    }

    const auto& config = getSettingFast<"wave-trail-size", WaveTrailSize>().get(this->isPlayer1());
    auto f = m_fields.self();
    
    if (!config.enabled) {
        if (f->didSetTrailSize) {
            f->didSetTrailSize = false;
            m_waveTrail->m_waveSize = f->originalTrailSize;
        }

        return;
    }

    if (!f->didSetTrailSize) {
        f->didSetTrailSize = true;
        f->originalTrailSize = m_waveTrail->m_waveSize;
    }
    
    m_waveTrail->m_waveSize = config.size * m_vehicleSize;
}

void ProPlayerObject::applyNoTrailBehind() {
    if (!this->isVanillaPlayer()) {
        return;
    }

    auto enabled = getSettingFast<"no-trail-behind", NoTrailBehind>().get(this->isPlayer1());
    auto f = m_fields.self();

    if (!enabled || !m_isDart) {
        if (f->didHideTrail) {
            f->didHideTrail = false;
            m_regularTrail->setVisible(true);
        }

        return;
    }

    if (m_regularTrail->isVisible()) {
        f->didHideTrail = true;
    }

    m_regularTrail->setVisible(false);
}

void ProPlayerObject::updateTrailRGB(float dt) {
    const auto& config = getSettingFast<"wave-rgb", WaveRGB>().get(this->isPlayer1());
    
    if (!config.enabled) {
        return;
    }

    auto f = m_fields.self();

    auto duration = 5.f / config.speed;

    f->rgbTime += dt;

    if (f->rgbTime > duration) {
        f->rgbTime = f->rgbTime - duration;
    }

    auto t = f->rgbTime / duration;

    auto r = clampf(std::abs(6.f * t - 3.f) - 1.f, 0.f, 1.f);
    auto g = clampf(2.f - std::abs(6.f * t - 2.f), 0.f, 1.f);
    auto b = clampf(2.f - std::abs(6.f * t - 4.f), 0.f, 1.f);
    auto pastel = 1.f - config.saturation;

    r = r + (1.f - r) * pastel;
    g = g + (1.f - g) * pastel;
    b = b + (1.f - b) * pastel;

    auto color = ccc3(
        r * 255,
        g * 255,
        b * 255
    );

    m_waveTrail->setColor(color);
}

void ProPlayerObject::update(float dt) {
    PlayerObject::update(dt);
    this->applyWavePulseSize();
    this->applyNoWavePulse();
}

void ProPlayerObject::updateStreakBlend(bool p0) {
    PlayerObject::updateStreakBlend(p0);
    this->applySolidWaveTrail();
}

void ProPlayerObject::setupStreak() {
    PlayerObject::setupStreak();
    this->applySolidWaveTrail();
}

void ProPlayerObject::resetStreak() {
    PlayerObject::resetStreak();
    this->applySolidWaveTrail();
}

void ProPlayerObject::fadeOutStreak2(float p0) {
    if (!this->isVanillaPlayer() || !m_isDart) {
        PlayerObject::fadeOutStreak2(p0);
        return;
    }

    if (!getSettingFast<"keep-trail-on-death", KeepTrailOnDeath>().get(this->isPlayer1())) {
        PlayerObject::fadeOutStreak2(p0);
    }
}

void ProPlayerObject::togglePlayerScale(bool p0, bool p1) {
    PlayerObject::togglePlayerScale(p0, p1);
    this->applyWaveTrailSize();
}

void ProPlayerObject::switchedToMode(GameObjectType p0) {
    PlayerObject::switchedToMode(p0);
    this->applyNoTrailBehind();
}