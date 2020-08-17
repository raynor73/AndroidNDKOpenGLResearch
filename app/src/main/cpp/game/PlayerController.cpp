//
// Created by Igor Lapin on 17/08/2020.
//

#include <engine_3d/Utils.h>
#include <engine_3d/Constants.h>
#include "PlayerController.h"

using namespace Engine3D::Utils;

PlayerController::PlayerController(
        std::shared_ptr<PhysicsEngine> physicsEngine,
        std::shared_ptr<DisplayInfo> displayInfo,
        std::shared_ptr<SimpleJoystick> movementJoystick,
        std::shared_ptr<ScrollDetectorComponent> playerViewDirectionScrollDetector,
        std::shared_ptr<TransformationComponent> playerRotorTransform
) : DisplayInfoUpdateDetector(std::move(displayInfo)),
    m_physicsEngine(std::move(physicsEngine)),
    m_movementJoystick(std::move(movementJoystick)),
    m_playerViewDirectionScrollDetector(std::move(playerViewDirectionScrollDetector)),
    m_playerRotorTransform(std::move(playerRotorTransform))
{
    m_cameraMovementMaxSpeed = 0.1; // units per second
}

void PlayerController::update() {
    if (!m_isEnabled) {
        return;
    }

    if (isDisplayInfoUpdated()) {
        // around 180 degrees per screen half-width scroll gesture
        m_cameraRotationSensitivity = 360 / m_displayInfo->width();
    }

    auto optionalScrollEvent = m_playerViewDirectionScrollDetector->scrollEvent();
    if (optionalScrollEvent) {
        auto scrollEvent = optionalScrollEvent.value();
        m_playerAngle += glm::radians(-scrollEvent.dx * m_cameraRotationSensitivity);
    }
    if (m_movementJoystick->position().y > 0.5f) {
        m_physicsEngine->setRigidBodyFriction("player", 0.001);
        auto velocity = m_physicsEngine->getRigidBodyVelocity("player");
        if (glm::length(velocity) < 5) {
            auto forceDirection = eulerZXY(0, 0, m_playerAngle) * Engine3D::Constants::DEFAULT_FORWARD_DIRECTION;

            auto perpendicularToForceDirection = glm::vec3(forceDirection.z, forceDirection.y, -forceDirection.x);
            auto velocityProjectionOnPerpendicularForceDirection =
                    glm::dot(velocity, perpendicularToForceDirection) / glm::length(perpendicularToForceDirection);
            auto unwantedVelocityCompensationForce =
                    -perpendicularToForceDirection * velocityProjectionOnPerpendicularForceDirection;

            forceDirection += unwantedVelocityCompensationForce;
            m_physicsEngine->addForce("player", forceDirection * 100.0f);
        }
    } else {
        m_physicsEngine->setRigidBodyFriction("player", 100);
    }
    m_playerRotorTransform->setRotation(eulerZXY(0, 0, m_playerAngle));
}
