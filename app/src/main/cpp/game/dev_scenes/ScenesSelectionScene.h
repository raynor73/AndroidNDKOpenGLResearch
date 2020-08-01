//
// Created by Igor Lapin on 29/07/2020.
//

#ifndef GAME_SCENES_SELECTION_SCENE_H
#define GAME_SCENES_SELECTION_SCENE_H


#include <game/Scene.h>
#include <game/touch_screen/ClickDetectorComponent.h>
#include <engine_3d/TextComponent.h>

class ScenesSelectionScene : public Scene {

    std::shared_ptr<ClickDetectorComponent> m_renderingEngineDevSceneMenuItemClickDetector;
    std::shared_ptr<TextComponent> m_renderingEngineDevSceneMenuItemTextComponent;

    std::shared_ptr<ClickDetectorComponent> m_screenBlinkingSceneMenuItemClickDetector;
    std::shared_ptr<TextComponent> m_screenBlinkingSceneMenuItemTextComponent;

public:
    ScenesSelectionScene(
            std::shared_ptr<TimeProvider> timeProvider,
            std::shared_ptr<DisplayInfo> displayInfo,
            std::shared_ptr<UnitsConverter> unitsConverter,
            std::shared_ptr<MeshLoadingRepository> meshLoadingRepository,
            std::shared_ptr<MeshRendererFactory> meshRendererFactory,
            std::shared_ptr<TextRendererFactory> textRendererFactory,
            std::shared_ptr<TouchScreen> touchScreen
    ) : Scene(
            timeProvider,
            displayInfo,
            unitsConverter,
            meshLoadingRepository,
            meshRendererFactory,
            textRendererFactory,
            touchScreen
    ) {}

    virtual void update(float dt) override;

    virtual void restoreFromStateRepresentation(const std::string stateRepresentation) override;
};


#endif //GAME_SCENES_SELECTION_SCENE_H
