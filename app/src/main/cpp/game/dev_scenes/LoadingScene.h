//
// Created by Igor Lapin on 16/08/2020.
//

#ifndef GAME_LOADING_SCENE_H
#define GAME_LOADING_SCENE_H


#include <game/Scene.h>
#include <game/SceneManager.h>

class LoadingScene : public Scene {

    std::shared_ptr<SceneManager> m_sceneManager;

    SceneType m_loadingSceneType;

public:
    static const std::string LOADING_SCENE_TYPE_KEY;

    LoadingScene(
            std::unordered_map<std::string, RequestedSceneArgValue> args,
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
            std::shared_ptr<SkeletalAnimationLoadingRepository> skeletalAnimationRepository,
            std::shared_ptr<SoundLoadingRepository> soundLoadingRepository,
            std::shared_ptr<SoundStorage> soundStorage,
            std::shared_ptr<SoundScene> soundScene
    );

protected:
    virtual void update(float dt) override;
};


#endif //GAME_LOADING_SCENE_H
