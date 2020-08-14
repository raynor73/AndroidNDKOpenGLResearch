//
// Created by Igor Lapin on 14/08/2020.
//

#include "CollisionsInfoComponent.h"

const std::string CollisionsInfoComponent::TYPE_NAME = "CollisionsInfoComponent";

std::shared_ptr<GameObjectComponent> CollisionsInfoComponent::clone() {
    auto clone = std::make_shared<CollisionsInfoComponent>();
    clone->setEnabled(m_isEnabled);
    return clone;
}
