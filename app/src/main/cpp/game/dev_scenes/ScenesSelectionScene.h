//
// Created by Igor Lapin on 29/07/2020.
//

#ifndef GAME_SCENES_SELECTION_SCENE_H
#define GAME_SCENES_SELECTION_SCENE_H


#include <game/Scene.h>
#include <game/touch_screen/ClickDetectorComponent.h>
#include <engine_3d/TextComponent.h>
#include <game/SceneManager.h>

class ScenesSelectionScene : public Scene {

    std::shared_ptr<ClickDetectorComponent> m_renderingEngineDevSceneMenuItemClickDetector;
    std::shared_ptr<ClickDetectorComponent> m_screenBlinkingSceneMenuItemClickDetector;
    std::shared_ptr<ClickDetectorComponent> m_multitouchTestSceneMenuItemClickDetector;

    std::shared_ptr<SceneManager> m_sceneManager;

public:
    ScenesSelectionScene(
            std::shared_ptr<TimeProvider> timeProvider,
            std::shared_ptr<DisplayInfo> displayInfo,
            std::shared_ptr<UnitsConverter> unitsConverter,
            std::shared_ptr<MeshLoadingRepository> meshLoadingRepository,
            std::shared_ptr<MeshRendererFactory> meshRendererFactory,
            std::shared_ptr<TextRendererFactory> textRendererFactory,
            std::shared_ptr<TouchScreen> touchScreen,
            std::shared_ptr<SceneManager> sceneManager,
            std::shared_ptr<TexturesRepository> texturesRepository,
            std::shared_ptr<PhysicsEngine> physicsEngine,
            std::shared_ptr<SkeletalAnimationLoadingRepository> skeletalAnimationRepository
    ) : Scene(
            timeProvider,
            displayInfo,
            unitsConverter,
            meshLoadingRepository,
            meshRendererFactory,
            textRendererFactory,
            touchScreen,
            texturesRepository,
            physicsEngine,
            skeletalAnimationRepository
    ), m_sceneManager(sceneManager) {}

    virtual void update(float dt) override;

    virtual void restoreStaticStateFromRepresentation(const std::string& stateRepresentation) override;
};


#endif //GAME_SCENES_SELECTION_SCENE_H
