//
// Created by Igor Lapin on 15/08/2020.
//

#include <utility>
#include <engine_3d/Constants.h>
#include <glm/gtc/quaternion.hpp>
#include "FreeFlyCameraController.h"

FreeFlyCameraController::FreeFlyCameraController(
        const std::shared_ptr<DisplayInfo>& displayInfo,
        std::shared_ptr<TransformationComponent> cameraTransform,
        std::shared_ptr<SimpleJoystick> movementJoystick,
        std::shared_ptr<ScrollDetectorComponent> viewDirectionScrollDetector
) : m_cameraTransform(std::move(cameraTransform)),
    m_movementJoystick(std::move(movementJoystick)),
    m_rightControllerAreaScrollDetector(std::move(viewDirectionScrollDetector))
{
    m_cameraRotationSensitivity = 360 / displayInfo->width(); // around 180 degrees per screen half-width scroll gesture
    m_cameraMovementMaxSpeed = 0.1; // units per second
}

void FreeFlyCameraController::update() {
    if (!m_isEnabled) {
        return;
    }

    m_movementJoystick->update();

    auto optionalRightControllerScrollEvent = m_rightControllerAreaScrollDetector->scrollEvent();
    if (optionalRightControllerScrollEvent) {
        auto scrollEvent = optionalRightControllerScrollEvent.value();

        m_cameraAngleX += glm::radians(scrollEvent.dy* m_cameraRotationSensitivity);
        m_cameraAngleY += glm::radians(-scrollEvent.dx * m_cameraRotationSensitivity);

        auto rotation = glm::rotate(
                glm::identity<glm::quat>(),
                m_cameraAngleY,
                glm::vec3(0, 1, 0)
        );
        rotation = glm::rotate(
                rotation,
                m_cameraAngleX,
                glm::vec3(1, 0, 0)
        );
        m_cameraTransform->setRotation(rotation);
    }

    auto movementJoystickPosition = m_movementJoystick->position();
    if (abs(movementJoystickPosition.y) > 0.01) {
        auto rotation = m_cameraTransform->rotation();
        glm::vec3 movement =
                rotation *
                Engine3D::Constants::DEFAULT_FORWARD_DIRECTION *
                m_cameraMovementMaxSpeed *
                movementJoystickPosition.y;
        m_cameraTransform->setPosition(m_cameraTransform->position() + movement);
    }
    if (abs(movementJoystickPosition.x) > 0.01) {
        auto rotation = m_cameraTransform->rotation();
        glm::vec3 strafe =
                rotation *
                glm::cross(Engine3D::Constants::DEFAULT_FORWARD_DIRECTION, Engine3D::Constants::CAMERA_UP_DIRECTION) *
                m_cameraMovementMaxSpeed *
                movementJoystickPosition.x;
        m_cameraTransform->setPosition(m_cameraTransform->position() + strafe);
    }
}
