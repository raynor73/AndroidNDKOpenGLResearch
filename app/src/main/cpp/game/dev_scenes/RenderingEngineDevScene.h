//
// Created by Igor Lapin on 28/06/2020.
//

#ifndef GAME_RENDERINGENGINEDEVSCENE_H
#define GAME_RENDERINGENGINEDEVSCENE_H


#include <game/Scene.h>
#include <memory>
#include <game/FpsCalculator.h>

class RenderingEngineDevScene : public Scene {

    FpsCalculator m_fpsCalculator;

public:
    RenderingEngineDevScene(
            std::shared_ptr<TimeProvider> timeProvider,
            std::shared_ptr<DisplayInfo> displayInfo,
            std::shared_ptr<UnitsConverter> unitsConverter,
            std::shared_ptr<MeshLoadingRepository> meshLoadingRepository,
            std::shared_ptr<MeshRendererFactory> meshRendererFactory,
            std::shared_ptr<TextRendererFactory> textRendererFactory,
            std::shared_ptr<TouchScreen> touchScreen,
            std::shared_ptr<TexturesRepository> texturesRepository
    ) : Scene(
            timeProvider,
            displayInfo,
            unitsConverter,
            meshLoadingRepository,
            meshRendererFactory,
            textRendererFactory,
            touchScreen,
            texturesRepository
    ) {}

    virtual void update(float dt) override;
};


#endif //GAME_RENDERINGENGINEDEVSCENE_H
