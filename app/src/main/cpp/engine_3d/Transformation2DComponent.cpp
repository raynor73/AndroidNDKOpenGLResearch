//
// Created by Igor Lapin on 07/08/2020.
//

#include <sstream>
#include "LayoutComponent.h"
#include "Transformation2DComponent.h"
#include "TransformationComponent.h"
#include "GameObject.h"
#include "Utils.h"

using namespace Engine3D::Utils;

const std::string Transformation2DComponent::TYPE_NAME = "Transformation2DComponent";

std::shared_ptr <GameObjectComponent> Transformation2DComponent::clone() {
    auto clone = std::make_shared<Transformation2DComponent>(
            m_unitsConverter,
            m_displayInfo,
            m_positionX,
            m_positionY,
            m_scaleX,
            m_scaleY
    );
    clone->setEnabled(m_isEnabled);
    return clone;
}

void Transformation2DComponent::update() {
    GameObjectComponent::update();

    if (!m_isEnabled) {
        return;
    }

    throwErrorIfNoGameObject();

    auto transform3D = std::static_pointer_cast<TransformationComponent>(
            m_gameObject->findComponent(TransformationComponent::TYPE_NAME)
    );
    throwErrorIfNull(transform3D, "No transformation component for applying 2D transform");

    if (m_gameObject->findComponent(LayoutComponent::TYPE_NAME) == nullptr) {
        transform3D->setPosition(glm::vec3(
                m_unitsConverter->complexValueToPixels(m_positionX),
                m_unitsConverter->complexValueToPixels(m_positionY),
                transform3D->position().z
        ));
    }

    transform3D->setScale(glm::vec3(
            m_unitsConverter->complexValueToPixels(m_scaleX),
            m_unitsConverter->complexValueToPixels(m_scaleY),
            transform3D->scale().z
    ));
}
