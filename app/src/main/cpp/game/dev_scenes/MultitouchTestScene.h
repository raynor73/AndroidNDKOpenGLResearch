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

class MultitouchTestScene : public Scene {

    std::shared_ptr<SceneManager> m_sceneManager;

    std::shared_ptr<SceneCloser> m_sceneCloser;
    ObjectPool<GameObject> m_touchIndicatorPrefabs;

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
            std::shared_ptr<SceneManager> sceneManager
    );
};


#endif //GAME_MULTITOUCH_TEST_SCENE_H
