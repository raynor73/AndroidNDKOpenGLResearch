//
// Created by Igor Lapin on 28/06/2020.
//

#ifndef ENGINE_3D_ORTHOCAMERACOMPONENT_H
#define ENGINE_3D_ORTHOCAMERACOMPONENT_H


#include "UnitsConverter.h"
#include "DisplayInfo.h"
#include "DisplayInfoUpdateDetector.h"
#include <utility>
#include <memory>
#include "CameraComponent.h"

class OrthoCameraComponent : public CameraComponent, DisplayInfoUpdateDetector {

    std::shared_ptr<UnitsConverter> m_unitsConverter;

    ComplexValue m_left;
    ComplexValue m_top;
    ComplexValue m_right;
    ComplexValue m_bottom;

    glm::mat4 m_projectionMatrix;

public:
    static const std::string TYPE_NAME;

    OrthoCameraComponent(
            std::shared_ptr<DisplayInfo> displayInfo,
            std::shared_ptr<UnitsConverter> unitsConverter,
            const glm::vec4& clearColor,
            const std::vector<std::string>& layerNames,
            ComplexValue left,
            ComplexValue top,
            ComplexValue right,
            ComplexValue bottom,
            float zNear,
            float zFar,
            int order
    ) : CameraComponent(clearColor, layerNames, zNear, zFar, order),
        DisplayInfoUpdateDetector(displayInfo),
        m_unitsConverter(std::move(unitsConverter)),
        m_left(std::move(left)),
        m_top(std::move(top)),
        m_right(std::move(right)),
        m_bottom(std::move(bottom)) {}

    glm::mat4 calculateViewMatrix() override;
    glm::mat4 calculateProjectionMatrix() override;
    virtual const std::string& typeName() const override { return OrthoCameraComponent::TYPE_NAME; }
    virtual std::shared_ptr<GameObjectComponent> clone() override;
};


#endif //ENGINE_3D_ORTHOCAMERACOMPONENT_H
