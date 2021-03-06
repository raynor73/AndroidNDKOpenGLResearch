//
// Created by Igor Lapin on 03/08/2020.
//

#ifndef GAME_MULTITOUCH_TEST_SCENE_H
#define GAME_MULTITOUCH_TEST_SCENE_H


#include <engine_3d/GameObject.h>
#include <engine_3d/ObjectPool.h>
#include <engine_3d/Scene.h>
#include <engine_3d/SceneManager.h>
#include <game/dev_scenes/SceneCloser.h>
#include <utility>
#include <game/FpsCalculator.h>
#include <unordered_map>
#include <engine_3d/AppStateRepository.h>

class MultitouchTestScene : public Scene {

    std::shared_ptr<SceneManager> m_sceneManager;

    std::shared_ptr<SceneCloser> m_sceneCloser;
    ObjectPool<GameObject> m_touchIndicatorsPool;

    FpsCalculator m_fpsCalculator;
    std::shared_ptr<GameObject> m_touchIndicatorPrefab;
    std::shared_ptr<GestureConsumerComponent> m_rootGestureConsumer;

    std::unordered_map<int, std::shared_ptr<GameObject>> m_activeTouchIndicators;

public:
    MultitouchTestScene(
            std::shared_ptr<Time> time,
            std::shared_ptr<DisplayInfo> displayInfo,
            std::shared_ptr<UnitsConverter> unitsConverter,
            std::shared_ptr<MeshLoadingRepository> meshLoadingRepository,
            std::shared_ptr<MeshRendererFactory> meshRendererFactory,
            std::shared_ptr<TextRendererFactory> textRendererFactory,
            std::shared_ptr<TouchScreen> touchScreen,
            std::shared_ptr<TexturesRepository> texturesRepository,
            std::shared_ptr<SceneManager> sceneManager,
            std::shared_ptr<PhysicsEngine> physicsEngine,
            std::shared_ptr<SkeletalAnimationLoadingRepository> skeletalAnimationRepository,
            std::shared_ptr<SoundLoadingRepository> soundLoadingRepository,
            std::shared_ptr<SoundStorage> soundStorage,
            std::shared_ptr<SoundScene> soundScene,
            std::shared_ptr<AppStateRepository> appStateRepository
    );

    virtual void buildHierarchyFromRepresentation(const std::string& hierarchyRepresentation) override;

protected:
    virtual void update() override;
};


#endif //GAME_MULTITOUCH_TEST_SCENE_H
