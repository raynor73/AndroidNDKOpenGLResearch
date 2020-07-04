//
// Created by Igor Lapin on 04/07/2020.
//

#include "AmbientLightComponent.h"

const std::string AmbientLightComponent::TYPE_NAME = "AmbientLightComponent";

std::shared_ptr<GameObjectComponent> AmbientLightComponent::clone() {
    auto clone = std::make_shared<AmbientLightComponent>(
            m_color,
            m_layerNames
    );
    clone->setEnabled(m_isEnabled);
    return clone;
}
