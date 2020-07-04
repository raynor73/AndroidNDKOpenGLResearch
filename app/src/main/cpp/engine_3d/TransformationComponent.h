//
// Created by Igor Lapin on 27/06/2020.
//

#ifndef ENGINE_3D_TRANSFORMATIONCOMPONENT_H
#define ENGINE_3D_TRANSFORMATIONCOMPONENT_H


#include <glm/glm.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <string>
#include "GameObjectComponent.h"

class TransformationComponent : public GameObjectComponent {

    bool m_isDirty = false;

    glm::vec3 m_position;
    glm::quat m_rotation;
    glm::vec3 m_scale;

    glm::vec3 m_finalPosition;
    glm::quat m_finalRotation;
    glm::vec3 m_finalScale;

    void setDirty();
    void calculateFinalTransformation();

public:
    static const std::string TYPE_NAME;

    TransformationComponent(
            const glm::vec3& position,
            const glm::quat& rotation,
            const glm::vec3& scale
    ) : m_position(position), m_rotation(rotation), m_scale(scale) {}

    virtual const std::string& typeName() const override { return TransformationComponent::TYPE_NAME; }

    const glm::vec3& localPosition() const { return m_position; }
    const glm::quat& localRotation() const { return m_rotation; }
    const glm::vec3& localScale() const { return m_scale; }

    const glm::vec3& position();
    void setPosition(const glm::vec3& position);
    const glm::quat& rotation();
    void setRotation(const glm::quat& rotation);
    const glm::vec3& scale();
    void setScale(const glm::vec3& scale);

    virtual std::shared_ptr<GameObjectComponent> clone() override;
};


#endif //ENGINE_3D_TRANSFORMATIONCOMPONENT_H
