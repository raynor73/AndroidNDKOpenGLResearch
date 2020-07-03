//
// Created by Igor Lapin on 03/07/2020.
//

#include "AndroidMeshRendererComponent.h"

const std::string AndroidMeshRendererComponent::TYPE_NAME = "AndroidMeshRendererComponent";

std::shared_ptr<GameObjectComponent> AndroidMeshRendererComponent::clone() {
    return std::shared_ptr<GameObjectComponent>();
}
