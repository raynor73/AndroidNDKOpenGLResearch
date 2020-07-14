//
// Created by Igor Lapin on 03/07/2020.
//

#include "main/rendering_engine/OpenGlMeshRendererComponent.h"
#include "OpenGlMeshRendererFactory.h"

std::shared_ptr<GameObjectComponent> OpenGlMeshRendererFactory::createMeshRenderer(
        std::vector<std::string> layerNames
) {
    return std::make_shared<OpenGlMeshRendererComponent>(layerNames);
}
