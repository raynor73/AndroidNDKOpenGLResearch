//
// Created by Igor Lapin on 09/07/2020.
//

#include "MaterialComponent.h"

const std::string MaterialComponent::TYPE_NAME = "MaterialComponent";

std::shared_ptr<GameObjectComponent> MaterialComponent::clone() {
    auto clone = std::make_shared<MaterialComponent>(m_material);
    clone->setEnabled(m_isEnabled);
    return clone;
}
