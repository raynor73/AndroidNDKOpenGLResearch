//
// Created by Igor Lapin on 28/06/2020.
//

#include <engine_3d/TextComponent.h>
#include <engine_3d/Utils.h>
#include "RenderingEngineDevScene.h"

using namespace Engine3D::Utils;

void RenderingEngineDevScene::update(float dt) {
    m_fpsCalculator.update(dt);

    if (m_closeButtonClickDetector->isClickDetected()) {
        m_sceneManager->requestSceneLoadAndStart(SceneType::SCENES_SELECTION_SCENE);
    }

    auto fpsText = std::static_pointer_cast<TextComponent>(
            m_gameObjectsMap["fpsText"]->findComponent(TextComponent::TYPE_NAME)
    );

    {
        std::stringstream ss;
        ss  << "FPS: " << int(m_fpsCalculator.fps());
        fpsText->setText(ss.str());
    }
}

void RenderingEngineDevScene::restoreFromStateRepresentation(const std::string stateRepresentation) {
    Scene::restoreFromStateRepresentation(stateRepresentation);

    auto closeButtonGameObject = m_gameObjectsMap.at("closeButton");
    m_closeButtonClickDetector = std::static_pointer_cast<ClickDetectorComponent>(
            closeButtonGameObject->findComponent(ClickDetectorComponent::TYPE_NAME)
    );
    throwErrorIfNull(
            m_closeButtonClickDetector,
            "Click detector for Rendering Engine Dev Scene close button not found"
    );
}
