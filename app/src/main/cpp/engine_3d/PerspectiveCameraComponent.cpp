//
// Created by Igor Lapin on 08/08/2020.
//

#include "PerspectiveCameraComponent.h"
#include "TransformationComponent.h"
#include "Constants.h"
#include "GameObject.h"

const std::string PerspectiveCameraComponent::TYPE_NAME = "PerspectiveCameraComponent";

glm::mat4 PerspectiveCameraComponent::calculateViewMatrix() {
    throwErrorIfNoGameObject();

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

glm::mat4 PerspectiveCameraComponent::calculateProjectionMatrix() {
    if (m_lastViewportWidth != m_viewportWidth || m_lastViewportHeight != m_viewportHeight) {
        m_lastViewportWidth = m_viewportWidth;
        m_lastViewportHeight = m_viewportHeight;

        m_projectionMatrix = glm::perspective(
                m_fov,
                m_unitsConverter->complexValueToPixels(m_lastViewportWidth) /
                m_unitsConverter->complexValueToPixels(m_lastViewportHeight),
                m_zNear,
                m_zFar
        );
    }

    return m_projectionMatrix;
}

std::shared_ptr<GameObjectComponent> PerspectiveCameraComponent::clone() {
    auto clone = std::make_shared<PerspectiveCameraComponent>(
            m_unitsConverter,
            m_clearColor,
            m_layerNames,
            m_fov,
            m_zNear,
            m_zFar,
            m_order
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
