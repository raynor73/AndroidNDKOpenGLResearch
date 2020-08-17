//
// Created by Igor Lapin on 17/08/2020.
//

#ifndef GAME_PLAYER_CONTROLLER_H
#define GAME_PLAYER_CONTROLLER_H


#include <game/touch_screen/ScrollDetectorComponent.h>
#include <engine_3d/TransformationComponent.h>
#include <engine_3d/PhysicsEngine.h>
#include "SimpleJoystick.h"
#include "DisplayInfoUpdateDetector.h"

class PlayerController : DisplayInfoUpdateDetector {

    std::shared_ptr<PhysicsEngine> m_physicsEngine;

    std::shared_ptr<SimpleJoystick> m_movementJoystick;
    std::shared_ptr<ScrollDetectorComponent> m_playerViewDirectionScrollDetector;

    std::shared_ptr<TransformationComponent> m_playerRotorTransform;

    float m_cameraRotationSensitivity;
    float m_cameraMovementMaxSpeed;

    float m_playerAngle = 0;
    bool m_isEnabled;

public:
    PlayerController(
            std::shared_ptr<PhysicsEngine> m_physicsEngine,
            std::shared_ptr<DisplayInfo> displayInfo,
            std::shared_ptr<SimpleJoystick> movementJoystick,
            std::shared_ptr<ScrollDetectorComponent> playerViewDirectionScrollDetector,
            std::shared_ptr<TransformationComponent> playerRotorTransform
    );

    void setEnabled(bool isEnabled) { m_isEnabled = isEnabled; }

    void update();
};


#endif //GAME_PLAYER_CONTROLLER_H
