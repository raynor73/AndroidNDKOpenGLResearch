//
// Created by Igor Lapin on 08/08/2020.
//

#include "DirectionalLightComponent.h"

const std::string DirectionalLightComponent::TYPE_NAME = "DirectionalLightComponent";

std::shared_ptr<GameObjectComponent> DirectionalLightComponent::clone() {
    auto clone = std::make_shared<DirectionalLightComponent>(
            m_color,
            m_direction,
            m_layerNames
    );
    clone->setEnabled(m_isEnabled);
    return clone;
}

