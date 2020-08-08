//
// Created by Igor Lapin on 08/08/2020.
//

#ifndef ENGINE_3D_PERSPECTIVE_CAMERA_COMPONENT_H
#define ENGINE_3D_PERSPECTIVE_CAMERA_COMPONENT_H


#include <memory>
#include <utility>
#include <string>
#include <game/DisplayInfo.h>
#include "CameraComponent.h"

class PerspectiveCameraComponent : public CameraComponent {

    std::shared_ptr<UnitsConverter> m_unitsConverter;

    float m_fov;
    ComplexValue m_lastViewportWidth = PlainValue { NAN };
    ComplexValue m_lastViewportHeight = PlainValue { NAN };

    glm::mat4 m_projectionMatrix;

public:
    static const std::string TYPE_NAME;

    PerspectiveCameraComponent(
            std::shared_ptr<UnitsConverter> unitsConverter,
            const glm::vec4& clearColor,
            const std::vector<std::string>& layerNames,
            float fov,
            float zNear,
            float zFar,
            int order
    ) : CameraComponent(clearColor, layerNames, zNear, zFar, order),
        m_unitsConverter(std::move(unitsConverter)),
        m_fov(fov)
    {}

    virtual glm::mat4 calculateViewMatrix() override;
    virtual glm::mat4 calculateProjectionMatrix() override;

    virtual const std::string& typeName() const override { return TYPE_NAME; }

    virtual std::shared_ptr<GameObjectComponent> clone() override;
};


#endif //ENGINE_3D_PERSPECTIVE_CAMERA_COMPONENT_H
