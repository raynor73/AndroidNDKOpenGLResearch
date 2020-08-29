//
// Created by Igor Lapin on 16/08/2020.
//

#include "LoadingScene.h"

const std::string LoadingScene::LOADING_SCENE_TYPE_KEY = "LOADING_SCENE_TYPE";

LoadingScene::LoadingScene(
        std::unordered_map<std::string, RequestedSceneArgValue> args,
        std::shared_ptr<TimeProvider> timeProvider, std::shared_ptr<DisplayInfo> displayInfo,
        std::shared_ptr<UnitsConverter> unitsConverter,
        std::shared_ptr<MeshLoadingRepository> meshLoadingRepository,
        std::shared_ptr<MeshRendererFactory> meshRendererFactory,
        std::shared_ptr<TextRendererFactory> textRendererFactory,
        std::shared_ptr<TouchScreen> touchScreen,
        std::shared_ptr<TexturesRepository> texturesRepository,
        std::shared_ptr<SceneManager> sceneManager,
        std::shared_ptr<PhysicsEngine> physicsEngine,
        std::shared_ptr<SkeletalAnimationLoadingRepository> skeletalAnimationRepository,
        std::shared_ptr<SoundLoadingRepository> soundLoadingRepository
) : Scene(
        std::move(timeProvider),
        std::move(displayInfo),
        std::move(unitsConverter),
        std::move(meshLoadingRepository),
        std::move(meshRendererFactory),
        std::move(textRendererFactory),
        std::move(touchScreen),
        std::move(texturesRepository),
        std::move(physicsEngine),
        std::move(skeletalAnimationRepository),
        std::move(soundLoadingRepository)
),
    m_sceneManager(std::move(sceneManager))
{
    m_loadingSceneType = static_cast<SceneType>(std::get<int>(args.at(LOADING_SCENE_TYPE_KEY)));
}


void LoadingScene::update(float) {
    m_sceneManager->requestSceneLoadAndStart(m_loadingSceneType);
}
