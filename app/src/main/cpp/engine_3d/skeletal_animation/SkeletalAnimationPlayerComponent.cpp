//
// Created by Igor Lapin on 23/08/2020.
//

#include <glm/ext.hpp>
#include <engine_3d/GameObject.h>
#include <engine_3d/Utils.h>
#include "SkeletalAnimationPlayerComponent.h"
#include "SkeletalAnimationComponent.h"

using namespace Engine3D::Utils;

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

    throwErrorIfNoGameObject();

    auto skeletalAnimationComponent = m_gameObject->findComponent<SkeletalAnimationComponent>();
    throwErrorIfNull(skeletalAnimationComponent, [&]() {
        std::stringstream ss;
        ss << "No skeletal animation found for game object " << m_gameObject->name() << " while animating";
        return ss.str();
    });
    auto skeletalAnimation = skeletalAnimationComponent->skeletalAnimation();

    for (auto& joint : skeletalAnimation.joints) {
        m_preCalculatedJointWorldTransform.clear();
        calculateJointWorldTransform(*joint, skeletalAnimation.keyFrames[0]);
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

glm::mat4 SkeletalAnimationPlayerComponent::calculateJointWorldTransform(const Joint& joint, const KeyFrame& keyFrame) {
    if (m_preCalculatedJointWorldTransform.count(joint.name()) > 0) {
        return m_preCalculatedJointWorldTransform.at(joint.name());
    }

    auto parent = joint.parent();

    auto jointLocalTransform = keyFrame.jointLocalTransform.at(joint.name());
    if (parent == nullptr) {
        m_preCalculatedJointWorldTransform.insert({joint.name(), jointLocalTransform });
        return jointLocalTransform;
    }

    auto parentTransform = calculateJointWorldTransform(*parent, keyFrame);
    auto jointWorldTransform = parentTransform * jointLocalTransform;
    m_preCalculatedJointWorldTransform.insert({joint.name(), jointWorldTransform });

    m_jointTransforms[joint.index()] = jointWorldTransform * joint.invertedBindTransform();

    return jointWorldTransform;
}

