//
// Created by Igor Lapin on 27/06/2020.
//

#include <sstream>
#include <exception>
#include <memory>
#include "GameObject.h"
#include "TransformationComponent.h"

using namespace std;

const string TransformationComponent::TYPE_NAME = "TransformationComponent";

const glm::vec3 &TransformationComponent::position() {
    calculateFinalTransformation();
    return m_finalPosition;
}

void TransformationComponent::setPosition(const glm::vec3& position) {
    setDirty();
    m_position = position;
}

const glm::quat& TransformationComponent::rotation() {
    calculateFinalTransformation();
    return m_finalRotation;
}

void TransformationComponent::setRotation(const glm::quat& rotation) {
    setDirty();
    m_rotation = rotation;
}

const glm::vec3 &TransformationComponent::scale() {
    calculateFinalTransformation();
    return m_finalScale;
}

void TransformationComponent::setScale(const glm::vec3 &scale) {
    setDirty();
    m_scale = scale;
}

void TransformationComponent::setDirty() {
    if (m_gameObject == nullptr) {
        std::stringstream ss;
        ss << "Component " << typeName() << " has no parent game object";
        throw domain_error(ss.str());
    }

    auto children = m_gameObject->children();
    for (auto& entry : children) {
        auto childTransform = std::static_pointer_cast<TransformationComponent>(
                entry.second->findComponent(TransformationComponent::TYPE_NAME)
        );
        if (childTransform == nullptr) {
            std::stringstream msgBuilder;
            msgBuilder << "Game object " << entry.second->name() << " has no transform component";
            throw domain_error(msgBuilder.str());
        }
        childTransform->setDirty();
    }

    m_isDirty = true;
}

void TransformationComponent::calculateFinalTransformation() {
    if (m_isDirty) {
        if (m_gameObject == nullptr) {
            std::stringstream msgBuilder;
            msgBuilder << "Component " << TYPE_NAME << " has no parent game object";
            throw domain_error(msgBuilder.str());
        }
        auto parentGameObject = m_gameObject->parent();
        if (parentGameObject != nullptr) {
            auto parentTransform = std::static_pointer_cast<TransformationComponent>(
                    parentGameObject->findComponent(TYPE_NAME)
            );
            if (parentTransform == nullptr) {
                std::stringstream msgBuilder;
                msgBuilder << "Parent game object" << parentGameObject->name() << " has no transform component";
                throw domain_error(msgBuilder.str());
            }

            m_finalRotation = m_rotation * parentTransform->rotation();

            m_finalScale = m_scale * parentTransform->scale();

            m_finalPosition = parentTransform->rotation() * m_position;
            m_finalPosition += m_position;
        } else {
            m_finalPosition = m_position;
            m_finalRotation = m_rotation;
            m_finalScale = m_scale;
        }
        m_isDirty = false;
    }
}
