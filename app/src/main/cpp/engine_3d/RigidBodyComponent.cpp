//
// Created by Igor Lapin on 10/08/2020.
//

#include <glm/vec3.hpp>
#include <glm/mat4x3.hpp>
#include <sstream>
#include <engine_3d/Utils.h>
#include "GameObject.h"
#include "TransformationComponent.h"
#include "RigidBodyComponent.h"

using namespace Engine3D::Utils;

const std::string RigidBodyComponent::TYPE_NAME = "RigidBodyComponent";

RigidBodyComponent::~RigidBodyComponent() {
    m_physicsEngine->removeRigidBody(m_rigidBodyName);
}

void RigidBodyComponent::update() {
    GameObjectComponent::update();

    throwErrorIfNoGameObject();

    auto transform = std::static_pointer_cast<TransformationComponent>(
            m_gameObject->findComponent(TransformationComponent::TYPE_NAME)
    );
    throwErrorIfNull(transform, [&]() {
        std::stringstream ss;
        ss << "No transform found for game object " << m_gameObject->name() << " while updating rigid body";
        return ss.str();
    });

    glm::mat4x4 rotationMatrix;
    glm::vec3 position;
    m_physicsEngine->getRigidBodyRotationAndPosition(m_rigidBodyName, rotationMatrix, position);

    glm::quat rotationQuaternion = glm::quat_cast(rotationMatrix);

    transform->setPosition(position);
    transform->setRotation(rotationQuaternion);
}

void RigidBodyComponent::onGameObjectAttachedToParent() {
    GameObjectComponent::onGameObjectAttachedToParent();

    throwErrorIfNoGameObject();

    auto transform = m_gameObject->findComponent<TransformationComponent>();
    throwErrorIfNull(transform, "No transform to update rigid body");

    m_physicsEngine->setPosition(m_rigidBodyName, transform->position());
    m_physicsEngine->setRotation(m_rigidBodyName, transform->rotation());
    m_physicsEngine->setRigidBodyEnabled(m_rigidBodyName, true);
}

void RigidBodyComponent::onGameObjectDetachedFromParent() {
    GameObjectComponent::onGameObjectDetachedFromParent();

    m_physicsEngine->setRigidBodyEnabled(m_rigidBodyName, false);
}

std::shared_ptr <GameObjectComponent> RigidBodyComponent::clone() {
    auto clone = std::make_shared<RigidBodyComponent>(
            m_rigidBodyName,
            m_physicsEngine
    );
    clone->setEnabled(m_isEnabled);
    return clone;
}

