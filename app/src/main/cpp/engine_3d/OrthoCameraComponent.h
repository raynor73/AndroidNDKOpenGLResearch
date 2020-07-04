//
// Created by Igor Lapin on 28/06/2020.
//

#ifndef ENGINE_3D_ORTHOCAMERACOMPONENT_H
#define ENGINE_3D_ORTHOCAMERACOMPONENT_H


#include "CameraComponent.h"

class OrthoCameraComponent : public CameraComponent {

    float m_left;
    float m_top;
    float m_right;
    float m_bottom;

public:
    static const std::string TYPE_NAME;

    OrthoCameraComponent(
            const glm::vec4& clearColor,
            const std::vector<std::string>& layerNames,
            float left,
            float top,
            float right,
            float bottom,
            float zNear,
            float zFar
    ) : CameraComponent(clearColor, layerNames, zNear, zFar) {}

    glm::mat4 calculateViewMatrix() override;
    glm::mat4 calculateProjectionMatrix() override;
    virtual const std::string& typeName() const override { return OrthoCameraComponent::TYPE_NAME; }
    virtual std::shared_ptr<GameObjectComponent> clone() override;
};


#endif //ENGINE_3D_ORTHOCAMERACOMPONENT_H
