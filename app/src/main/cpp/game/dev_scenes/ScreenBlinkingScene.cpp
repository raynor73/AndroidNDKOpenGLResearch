//
// Created by Igor Lapin on 06/06/2020.
//

#include <GLES2/gl2.h>

#include <utility>
#include "ScreenBlinkingScene.h"

ScreenBlinkingScene::ScreenBlinkingScene(
        std::shared_ptr<TimeProvider> timeProvider,
        std::shared_ptr<DisplayInfo> displayInfo,
        std::shared_ptr<UnitsConverter> unitsConverter,
        std::shared_ptr<MeshLoadingRepository> meshLoadingRepository,
        std::shared_ptr<MeshRendererFactory> meshRendererFactory,
        std::shared_ptr<TextRendererFactory> textRendererFactory,
        std::shared_ptr<TouchScreen> touchScreen,
        std::shared_ptr<TexturesRepository> texturesRepository,
        std::shared_ptr<PhysicsEngine> physicsEngine,
        std::shared_ptr<SkeletalAnimationLoadingRepository> skeletalAnimationRepository
) : m_elapsed(0),
    m_isWhite(true),
    Scene(
            std::move(timeProvider),
            std::move(displayInfo),
            std::move(unitsConverter),
            std::move(meshLoadingRepository),
            std::move(meshRendererFactory),
            std::move(textRendererFactory),
            std::move(touchScreen),
            std::move(texturesRepository),
            std::move(physicsEngine),
            std::move(skeletalAnimationRepository)
    )
{
    // do nothing
}

void ScreenBlinkingScene::update(float dt) {
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


