//
// Created by Igor Lapin on 03/07/2020.
//

#include "AndroidMeshRendererComponent.h"
#include "AndroidMeshRendererFactory.h"

std::shared_ptr<GameObjectComponent> AndroidMeshRendererFactory::createMeshRenderer(std::vector<std::string> layerNames) {
    auto meshRenderer = std::make_shared<AndroidMeshRendererComponent>();
    return std::shared_ptr<GameObjectComponent>();
}
