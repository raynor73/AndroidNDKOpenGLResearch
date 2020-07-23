//
// Created by Igor Lapin on 23/07/2020.
//

#ifndef GAME_TEXT_RENDERER_FACTORY_H
#define GAME_TEXT_RENDERER_FACTORY_H


#include <memory>
#include <vector>
#include <string>
#include <engine_3d/GameObjectComponent.h>

class TextRendererFactory {

public:
    virtual ~TextRendererFactory() = default;

    virtual std::shared_ptr<GameObjectComponent> createTextRenderer(std::vector<std::string> layerNames) = 0;
};


#endif //GAME_TEXT_RENDERER_FACTORY_H
