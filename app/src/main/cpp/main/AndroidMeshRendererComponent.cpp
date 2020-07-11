//
// Created by Igor Lapin on 03/07/2020.
//

#include "AndroidMeshRendererComponent.h"

const std::string AndroidMeshRendererComponent::TYPE_NAME = "AndroidMeshRendererComponent";

void AndroidMeshRendererComponent::render() {

}

std::shared_ptr<GameObjectComponent> AndroidMeshRendererComponent::clone() {
    auto clone = std::make_shared<AndroidMeshRendererComponent>(m_layerNames);
    clone->setEnabled(m_isEnabled);
    return clone;
}
