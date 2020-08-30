//
// Created by Igor Lapin on 30/08/2020.
//

#include "Utils.h"
#include "GameObject.h"
#include "SoundPlayerComponent.h"
#include "TransformationComponent.h"

using namespace Engine3D::Utils;

const std::string SoundPlayerComponent::TYPE_NAME = "SoundPlayerComponent";

SoundPlayerComponent::SoundPlayerComponent(
        std::shared_ptr<SoundScene> soundScene,
        std::string soundName,
        float gain,
        float pitch,
        float rolloffFactor,
        float referenceDistance,
        float maxDistance,
        bool isLooped,
        bool isRelativeToListener
) : m_soundScene(std::move(soundScene)),
    m_soundName(std::move(soundName)),
    m_gain(gain),
    m_pitch(pitch),
    m_rolloffFactor(rolloffFactor),
    m_referenceDistance(referenceDistance),
    m_maxDistance(maxDistance),
    m_isLooped(isLooped),
    m_isRelativeToListener(isRelativeToListener)
{}

void SoundPlayerComponent::setEnabled(bool isEnabled) {
    GameObjectComponent::setEnabled(isEnabled);

    if (m_isEnabled) {
        if (m_isPlaying) {
            m_soundScene->startSoundPlayer(m_playerName, m_isLooped);
        }
    } else {
        if (m_isPlaying) {
            m_soundScene->pauseSoundPlayer(m_playerName);
        }
    }
}

void SoundPlayerComponent::onAttachedToGameObject() {
    GameObjectComponent::onAttachedToGameObject();

    if (!m_isSoundPlayerAddedToScene) {
        m_isSoundPlayerAddedToScene = true;

        throwErrorIfNoGameObject();

        auto transform = m_gameObject->findComponent<TransformationComponent>();
        throwErrorIfNull(transform, "No transform");

        m_playerName = m_gameObject->name();

        m_soundScene->addSoundPlayer(
                m_playerName,
                m_soundName,
                transform->position(),
                m_gain,
                m_pitch,
                m_rolloffFactor,
                m_referenceDistance,
                m_maxDistance,
                m_isRelativeToListener
        );
    }
}

void SoundPlayerComponent::onDetachedFromGameObject() {
    GameObjectComponent::onDetachedFromGameObject();

    m_soundScene->removeSoundPlayer(m_playerName);
}

void SoundPlayerComponent::update() {
    GameObjectComponent::update();

    throwErrorIfNoGameObject();

    auto transform = m_gameObject->findComponent<TransformationComponent>();
    throwErrorIfNull(transform, "No transform");

    m_soundScene->setSoundPlayerPosition(m_playerName, transform->position());
    m_soundScene->setSoundPlayerGain(m_playerName, m_gain);
    m_soundScene->setSoundPlayerPitch(m_playerName, m_pitch);
}

void SoundPlayerComponent::play() {
    m_isPlaying = true;
    m_soundScene->startSoundPlayer(m_playerName, m_isLooped);
}

void SoundPlayerComponent::pause() {
    m_isPlaying = false;
    m_soundScene->pauseSoundPlayer(m_playerName);
}

void SoundPlayerComponent::resume() {
    m_isPlaying = true;
    m_soundScene->resumeSoundPlayer(m_playerName);
}

void SoundPlayerComponent::stop() {
    m_isPlaying = false;
    m_soundScene->stopSoundPlayer(m_playerName);
}

std::shared_ptr<GameObjectComponent> SoundPlayerComponent::clone() {
    auto clone = std::make_shared<SoundPlayerComponent>(
            m_soundScene,
            m_soundName,
            m_gain,
            m_pitch,
            m_rolloffFactor,
            m_referenceDistance,
            m_maxDistance,
            m_isLooped,
            m_isRelativeToListener
    );
    return std::shared_ptr<GameObjectComponent>();
}
