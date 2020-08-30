//
// Created by Igor Lapin on 30/08/2020.
//

#include "Utils.h"
#include "GameObject.h"
#include "SoundListenerComponent.h"
#include "TransformationComponent.h"
#include "Constants.h"

using namespace Engine3D::Utils;

const std::string SoundListenerComponent::TYPE_NAME = "SoundListenerComponent";

SoundListenerComponent::SoundListenerComponent(
        std::shared_ptr<SoundScene> soundScene
) : m_soundScene(std::move(soundScene))
{}

void SoundListenerComponent::update() {
    GameObjectComponent::update();

    if (!m_isEnabled) {
        return;
    }

    throwErrorIfNoGameObject();

    auto transform = m_gameObject->findComponent<TransformationComponent>();
    throwErrorIfNull(transform, "No transform");

    auto lookAt = transform->rotation() * Engine3D::Constants::DEFAULT_FORWARD_DIRECTION;
    auto up = transform->rotation() *  Engine3D::Constants::CAMERA_UP_DIRECTION;
    m_soundScene->setSoundListenerTransform(transform->position(), lookAt, up);
}

std::shared_ptr<GameObjectComponent> SoundListenerComponent::clone() {
    auto clone = std::make_shared<SoundListenerComponent>(m_soundScene);
    clone->setEnabled(m_isEnabled);
    return clone;
}

