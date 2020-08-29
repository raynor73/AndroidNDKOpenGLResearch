//
// Created by Igor Lapin on 06/06/2020.
//

#ifndef GAME_SCREENBLINKINGSCENE_H
#define GAME_SCREENBLINKINGSCENE_H


#include <memory>
#include <game/Scene.h>

class ScreenBlinkingScene : public Scene {

    float m_elapsed;
    bool m_isWhite;

public:
    explicit ScreenBlinkingScene(
            std::shared_ptr<TimeProvider> timeProvider,
            std::shared_ptr<DisplayInfo> displayInfo,
            std::shared_ptr<UnitsConverter> unitsConverter,
            std::shared_ptr<MeshLoadingRepository> meshLoadingRepository,
            std::shared_ptr<MeshRendererFactory> meshRendererFactory,
            std::shared_ptr<TextRendererFactory> textRendererFactory,
            std::shared_ptr<TouchScreen> touchScreen,
            std::shared_ptr<TexturesRepository> texturesRepository,
            std::shared_ptr<PhysicsEngine> physicsEngine,
            std::shared_ptr<SkeletalAnimationLoadingRepository> skeletalAnimationRepository,
            std::shared_ptr<SoundLoadingRepository> soundLoadingRepository
    );

protected:
    virtual void update(float dt) override;
};


#endif //GAME_SCREENBLINKINGSCENE_H
