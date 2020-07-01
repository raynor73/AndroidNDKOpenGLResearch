//
// Created by Igor Lapin on 28/06/2020.
//

#include <sstream>
#include <exception>
#include "Constants.h"
#include "OrthoCameraComponent.h"
#include "GameObject.h"
#include "TransformationComponent.h"

const std::string OrthoCameraComponent::TYPE_NAME = "OrthoCameraComponent";

glm::mat4 OrthoCameraComponent::calculateViewMatrix() {
    if (m_gameObject == nullptr) {
        std::stringstream ss;
        ss << "Component " << typeName() << " has no game object";
        throw std::domain_error(ss.str());
    }

    auto transform = std::static_pointer_cast<TransformationComponent>(
            m_gameObject->findComponent(TransformationComponent::TYPE_NAME)
    );
    if (transform == nullptr) {
        std::stringstream ss;
        ss << "No transform component for game object " << m_gameObject->name() << " found";
        throw std::domain_error(ss.str());
    }

    glm::vec3 lookAtDirection;
    glm::vec3 up;

    lookAtDirection = Engine3D::Constants::DEFAULT_FORWARD_DIRECTION;
    lookAtDirection = transform->rotation() * lookAtDirection;

    up = Engine3D::Constants::CAMERA_UP_DIRECTION;
    up = transform->rotation() * up;

    return glm::lookAt(transform->position(), transform->position() + lookAtDirection, up);
}

glm::mat4 OrthoCameraComponent::calculateProjectionMatrix() {
    return glm::ortho(m_left, m_right, m_bottom, m_top, m_zNear, m_zFar);
}

std::shared_ptr<GameObjectComponent> OrthoCameraComponent::clone() {
    auto clone = std::make_shared<OrthoCameraComponent>(
            m_clearColor,
            m_layerNames,
            m_left,
            m_top,
            m_right,
            m_bottom,
            m_zNear,
            m_zFar
    );
    clone->setEnabled(m_isEnabled);
    clone->setViewportX(m_viewportX);
    clone->setViewportY(m_viewportY);
    clone->setViewportWidth(m_viewportWidth);
    clone->setViewportHeight(m_viewportHeight);
    clone->setShouldClearDepth(m_shouldClearDepth);
    clone->setShouldClearColor(m_shouldClearColor);
    return clone;
}
