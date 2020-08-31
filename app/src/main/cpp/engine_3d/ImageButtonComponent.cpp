//
// Created by Igor Lapin on 03/08/2020.
//

#include <memory>
#include "Utils.h"
#include "GameObject.h"
#include "MaterialComponent.h"
#include "touch_screen/ClickDetectorComponent.h"
#include "ImageButtonComponent.h"

using namespace Engine3D::Utils;

const std::string ImageButtonComponent::TYPE_NAME = "ImageButtonComponent";

void ImageButtonComponent::update() {
    GameObjectComponent::update();

    throwErrorIfNoGameObject();

    auto materialComponent = std::static_pointer_cast<MaterialComponent>(
            m_gameObject->findComponent(MaterialComponent::TYPE_NAME)
    );
    throwErrorIfNull(materialComponent, "Material component for image button not found");

    if (!m_isEnabled) {
        materialComponent->setMaterial(m_material);
        return;
    }

    auto clickDetector = std::static_pointer_cast<ClickDetectorComponent>(
            m_gameObject->findComponent(ClickDetectorComponent::TYPE_NAME)
    );
    throwErrorIfNull(clickDetector, "Click detector for image button not found");

    if (clickDetector->isPressDetected()) {
        materialComponent->setMaterial(m_pressedMaterial);
    } else {
        materialComponent->setMaterial(m_material);
    }
}

std::shared_ptr<GameObjectComponent> ImageButtonComponent::clone() {
    auto clone = std::make_shared<ImageButtonComponent>(m_material, m_pressedMaterial);
    clone->setEnabled(m_isEnabled);
    return clone;
}

