//
// Created by Igor Lapin on 03/08/2020.
//

#ifndef GAME_MULTITOUCH_TEST_SCENE_H
#define GAME_MULTITOUCH_TEST_SCENE_H


#include <engine_3d/GameObject.h>
#include <game/ObjectPool.h>
#include <game/Scene.h>
#include <game/SceneManager.h>
#include <game/dev_scenes/SceneCloser.h>
#include <utility>
#include <game/FpsCalculator.h>
#include <unordered_map>

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
            std::shared_ptr<TimeProvider> timeProvider,
            std::shared_ptr<DisplayInfo> displayInfo,
            std::shared_ptr<UnitsConverter> unitsConverter,
            std::shared_ptr<MeshLoadingRepository> meshLoadingRepository,
            std::shared_ptr<MeshRendererFactory> meshRendererFactory,
            std::shared_ptr<TextRendererFactory> textRendererFactory,
            std::shared_ptr<TouchScreen> touchScreen,
            std::shared_ptr<TexturesRepository> texturesRepository,
            std::shared_ptr<SceneManager> sceneManager,
            std::shared_ptr<PhysicsEngine> physicsEngine,
            std::shared_ptr<SkeletalAnimationLoadingRepository> skeletalAnimationRepository
    );

    virtual void restoreStaticStateFromRepresentation(const std::string& stateRepresentation) override;

protected:
    virtual void update(float dt) override;
};


#endif //GAME_MULTITOUCH_TEST_SCENE_H
