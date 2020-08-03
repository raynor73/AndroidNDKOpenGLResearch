//
// Created by Igor Lapin on 28/06/2020.
//

#ifndef GAME_RENDERINGENGINEDEVSCENE_H
#define GAME_RENDERINGENGINEDEVSCENE_H


#include <game/Scene.h>
#include <memory>
#include <game/FpsCalculator.h>
#include <game/touch_screen/ClickDetectorComponent.h>
#include <game/SceneManager.h>

class RenderingEngineDevScene : public Scene {

    std::shared_ptr<SceneManager> m_sceneManager;

    std::shared_ptr<ClickDetectorComponent> m_closeButtonClickDetector;

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
            std::shared_ptr<TexturesRepository> texturesRepository,
            std::shared_ptr<SceneManager> sceneManager
    ) : Scene(
            timeProvider,
            displayInfo,
            unitsConverter,
            meshLoadingRepository,
            meshRendererFactory,
            textRendererFactory,
            touchScreen,
            texturesRepository
    ), m_sceneManager(std::move(sceneManager)) {}

    virtual void update(float dt) override;

    virtual void restoreFromStateRepresentation(const std::string stateRepresentation) override;
};


#endif //GAME_RENDERINGENGINEDEVSCENE_H
