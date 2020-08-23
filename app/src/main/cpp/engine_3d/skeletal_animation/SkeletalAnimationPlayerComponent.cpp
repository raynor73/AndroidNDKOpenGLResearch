//
// Created by Igor Lapin on 23/08/2020.
//

#include <glm/ext.hpp>
#include "SkeletalAnimationPlayerComponent.h"

const std::string SkeletalAnimationPlayerComponent::TYPE_NAME = "SkeletalAnimationPlayerComponent";

SkeletalAnimationPlayerComponent::SkeletalAnimationPlayerComponent() {
    m_jointTransforms.resize(Engine3D::Constants::MAX_JOINTS);
}

void SkeletalAnimationPlayerComponent::update() {
    GameObjectComponent::update();

    if (!m_isEnabled) {
        // TODO Enable GameObjectComponent::setEnabled override
        stop();
        return;
    }

    for (auto& jointTransform : m_jointTransforms) {
        jointTransform = glm::identity<glm::mat4>();
    }
}

void SkeletalAnimationPlayerComponent::play() {

}

void SkeletalAnimationPlayerComponent::stop() {

}

void SkeletalAnimationPlayerComponent::onDetachedFromGameObject() {
    GameObjectComponent::onDetachedFromGameObject();

    stop();
}

std::shared_ptr<GameObjectComponent> SkeletalAnimationPlayerComponent::clone() {
    auto clone = std::make_shared<SkeletalAnimationPlayerComponent>();
    clone->setEnabled(m_isEnabled);
    return clone;
}

