//
// Created by Igor Lapin on 06/06/2020.
//

#include <GLES2/gl2.h>

#include <utility>
#include "ScreenBlinkingScene.h"

ScreenBlinkingScene::ScreenBlinkingScene(
        std::shared_ptr<Time> time,
        std::shared_ptr<DisplayInfo> displayInfo,
        std::shared_ptr<UnitsConverter> unitsConverter,
        std::shared_ptr<MeshLoadingRepository> meshLoadingRepository,
        std::shared_ptr<MeshRendererFactory> meshRendererFactory,
        std::shared_ptr<TextRendererFactory> textRendererFactory,
        std::shared_ptr<TouchScreen> touchScreen,
        std::shared_ptr<TexturesRepository> texturesRepository,
        std::shared_ptr<PhysicsEngine> physicsEngine,
        std::shared_ptr<SkeletalAnimationLoadingRepository> skeletalAnimationRepository,
        std::shared_ptr<SoundLoadingRepository> soundLoadingRepository,
        std::shared_ptr<SoundStorage> soundStorage,
        std::shared_ptr<SoundScene> soundScene,
        std::shared_ptr<AppStateRepository> appStateRepository
) : m_elapsed(0),
    m_isWhite(true),
    Scene(
            std::move(time),
            std::move(displayInfo),
            std::move(unitsConverter),
            std::move(meshLoadingRepository),
            std::move(meshRendererFactory),
            std::move(textRendererFactory),
            std::move(touchScreen),
            std::move(texturesRepository),
            std::move(physicsEngine),
            std::move(skeletalAnimationRepository),
            std::move(soundLoadingRepository),
            std::move(soundStorage),
            std::move(soundScene),
            std::move(appStateRepository)
    )
{
    // do nothing
}

void ScreenBlinkingScene::update() {
    Scene::update();

    auto dt = m_time->deltaTime();

    glClear(GL_COLOR_BUFFER_BIT);
    if (m_isWhite) {
        glClearColor(1, 1, 1, 1);
    } else {
        glClearColor(0, 0, 0, 1);
    }

    if (m_elapsed >= 1) {
        m_elapsed = 0;
        m_isWhite = !m_isWhite;
    }

    m_elapsed += dt;
}


