//
// Created by Igor Lapin on 23/08/2020.
//

#include "SkeletalAnimationComponent.h"

const std::string SkeletalAnimationComponent::TYPE_NAME = "SkeletalAnimationComponent";

std::shared_ptr<GameObjectComponent> SkeletalAnimationComponent::clone() {
    auto clone = std::make_shared<SkeletalAnimationComponent>(
            m_skeletalAnimation,
            m_skeletalAnimationName
    );
    clone->setEnabled(m_isEnabled);
    return clone;
}