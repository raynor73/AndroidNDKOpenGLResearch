//
// Created by Igor Lapin on 03/07/2020.
//

#ifndef GAME_MESHRENDERERFACTORY_H
#define GAME_MESHRENDERERFACTORY_H


#include <memory>
#include <engine_3d/GameObjectComponent.h>

class MeshRendererFactory {

public:
    virtual ~MeshRendererFactory() = default;

    virtual std::shared_ptr<GameObjectComponent> createMeshRenderer() = 0;
};


#endif //GAME_MESHRENDERERFACTORY_H
