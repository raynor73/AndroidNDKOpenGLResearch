//
// Created by Igor Lapin on 17/08/2020.
//

#ifndef GAME_PLAYER_CONTROLLER_H
#define GAME_PLAYER_CONTROLLER_H


#include <engine_3d/touch_screen/ScrollDetectorComponent.h>
#include <engine_3d/TransformationComponent.h>
#include <engine_3d/PhysicsEngine.h>
#include "SimpleJoystick.h"
#include <engine_3d/DisplayInfoUpdateDetector.h>

class PlayerController : DisplayInfoUpdateDetector {

    std::shared_ptr<PhysicsEngine> m_physicsEngine;

    std::shared_ptr<SimpleJoystick> m_movementJoystick;
    std::shared_ptr<ScrollDetectorComponent> m_playerViewDirectionScrollDetector;

    std::shared_ptr<TransformationComponent> m_playerRotorTransform;

    std::shared_ptr<GameObject> m_playerRotor;
    std::shared_ptr<GameObject> m_femaleIdle;
    std::shared_ptr<GameObject> m_femaleRunning;

    float m_cameraRotationSensitivity;

    float m_playerAngle = 0;
    bool m_isEnabled;

public:
    PlayerController(
            std::shared_ptr<PhysicsEngine> m_physicsEngine,
            std::shared_ptr<DisplayInfo> displayInfo,
            std::shared_ptr<SimpleJoystick> movementJoystick,
            std::shared_ptr<ScrollDetectorComponent> playerViewDirectionScrollDetector,
            std::shared_ptr<TransformationComponent> playerRotorTransform,
            std::shared_ptr<GameObject> playerRotor,
            std::shared_ptr<GameObject> femaleIdle,
            std::shared_ptr<GameObject> femaleRunning
    );

    void setEnabled(bool isEnabled) { m_isEnabled = isEnabled; }

    void update();

    float playerAngle() const { return m_playerAngle; }
    void setPlayerAngle(float playerAngle) { m_playerAngle = playerAngle; }

private:
    void activateIdleAnimationIfNecessary();
    void activateRunningAnimationIfNecessary();
};


#endif //GAME_PLAYER_CONTROLLER_H
