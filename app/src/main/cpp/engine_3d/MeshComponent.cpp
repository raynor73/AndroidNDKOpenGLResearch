//
// Created by Igor Lapin on 04/07/2020.
//

#include <memory>
#include "MeshComponent.h"

const std::string MeshComponent::TYPE_NAME = "MeshComponent";

std::shared_ptr<GameObjectComponent> MeshComponent::clone() {
    auto clone = std::make_shared<MeshComponent>(m_mesh);
    clone->setEnabled(m_isEnabled);
    return clone;
}

