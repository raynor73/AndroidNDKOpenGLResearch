//
// Created by Igor Lapin on 29/07/2020.
//

#include <engine_3d/Utils.h>
#include <main/L.h>
#include "ScenesSelectionScene.h"

using namespace Engine3D::Utils;

void ScenesSelectionScene::update(float) {
    if (m_renderingEngineDevSceneMenuItemClickDetector->isClickDetected()) {
        L::d("!@£", "Click!!!");
    }
    if (m_renderingEngineDevSceneMenuItemClickDetector->isPressDetected()) {
        L::d("!@£", "Press!!!");
    }
}

void ScenesSelectionScene::restoreFromStateRepresentation(const std::string stateRepresentation) {
    Scene::restoreFromStateRepresentation(stateRepresentation);

    auto renderingEngineDevSceneMenuItemGameObject = m_gameObjectsMap.at("renderingEngineDevSceneMenuItem");
    auto screenBlinkingSceneMenuItemGameObject = m_gameObjectsMap.at("screenBlinkingSceneMenuItem");

    m_renderingEngineDevSceneMenuItemClickDetector = std::static_pointer_cast<ClickDetectorComponent>(
            renderingEngineDevSceneMenuItemGameObject->findComponent(ClickDetectorComponent::TYPE_NAME)
    );
    m_screenBlinkingSceneMenuItemClickDetector = std::static_pointer_cast<ClickDetectorComponent>(
            screenBlinkingSceneMenuItemGameObject->findComponent(ClickDetectorComponent::TYPE_NAME)
    );

    throwErrorIfNull(
            m_renderingEngineDevSceneMenuItemClickDetector,
            "Click detector for Rendering Engine Dev Scene menu item not found"
    );
    throwErrorIfNull(
            m_screenBlinkingSceneMenuItemClickDetector,
            "Click detector for Screen Blinking Scene menu item not found"
    );
}
