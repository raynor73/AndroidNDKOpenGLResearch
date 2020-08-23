//
// Created by Igor Lapin on 23/08/2020.
//

#include "SkeletalAnimationPlayerComponent.h"

const std::string SkeletalAnimationPlayerComponent::TYPE_NAME = "SkeletalAnimationPlayerComponent";

void SkeletalAnimationPlayerComponent::update() {
    GameObjectComponent::update();

    if (!m_isEnabled) {
        // TODO Enable GameObjectComponent::setEnabled override
        stop();
        return;
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

