//
// Created by Igor Lapin on 15/08/2020.
//

#ifndef GAME_FREE_FLY_CAMERA_CONTROLLER_H
#define GAME_FREE_FLY_CAMERA_CONTROLLER_H


#include <memory>
#include <game/touch_screen/ScrollDetectorComponent.h>
#include "SimpleJoystick.h"
#include "engine_3d/TransformationComponent.h"
#include "DisplayInfo.h"

class FreeFlyCameraController {

    std::shared_ptr<SimpleJoystick> m_movementJoystick;
    std::shared_ptr<ScrollDetectorComponent> m_rightControllerAreaScrollDetector;

    std::shared_ptr<TransformationComponent> m_cameraTransform;

    float m_cameraRotationSensitivity;
    float m_cameraMovementMaxSpeed;

    float m_cameraAngleX = 0;
    float m_cameraAngleY = 0;

    bool m_isEnabled;

public:
    FreeFlyCameraController(
            const std::shared_ptr<DisplayInfo>& displayInfo,
            std::shared_ptr<TransformationComponent> cameraTransform,
            std::shared_ptr<SimpleJoystick> movementJoystick,
            std::shared_ptr<ScrollDetectorComponent> viewDirectionScrollDetector
    );

    void setEnabled(bool isEnabled) { m_isEnabled = isEnabled; }

    float cameraAngleX() const { return m_cameraAngleX; }
    void setCameraAngleX(float cameraAngleX) { m_cameraAngleX = cameraAngleX; updateRotation(); };

    float cameraAngleY() const { return m_cameraAngleY; }
    void setCameraAngleY(float cameraAngleY) { m_cameraAngleY = cameraAngleY; updateRotation(); }

    void update();

private:
    void updateRotation();
};


#endif //GAME_FREE_FLY_CAMERA_CONTROLLER_H
