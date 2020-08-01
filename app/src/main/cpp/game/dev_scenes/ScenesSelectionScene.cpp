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

    /*if (m_renderingEngineDevSceneMenuItemClickDetector->isPressDetected()) {
        m_renderingEngineDevSceneMenuItemTextComponent->setTextColor(glm::vec4(1, 1, 1, 1));
    } else {
        m_renderingEngineDevSceneMenuItemTextComponent->setTextColor(glm::vec4(0.5, 0.5, 1, 1));
    }

    if (m_screenBlinkingSceneMenuItemClickDetector->isPressDetected()) {
        m_screenBlinkingSceneMenuItemTextComponent->setTextColor(glm::vec4(1, 1, 1, 1));
    } else {
        m_screenBlinkingSceneMenuItemTextComponent->setTextColor(glm::vec4(0.5, 0.5, 1, 1));
    }*/
}

void ScenesSelectionScene::restoreFromStateRepresentation(const std::string stateRepresentation) {
    Scene::restoreFromStateRepresentation(stateRepresentation);

    auto renderingEngineDevSceneMenuItemGameObject = m_gameObjectsMap.at("renderingEngineDevSceneMenuItem");
    auto screenBlinkingSceneMenuItemGameObject = m_gameObjectsMap.at("screenBlinkingSceneMenuItem");

    m_renderingEngineDevSceneMenuItemClickDetector = std::static_pointer_cast<ClickDetectorComponent>(
            renderingEngineDevSceneMenuItemGameObject->findComponent(ClickDetectorComponent::TYPE_NAME)
    );
    throwErrorIfNull(
            m_renderingEngineDevSceneMenuItemClickDetector,
            "Click detector for Rendering Engine Dev Scene menu item not found"
    );

    m_screenBlinkingSceneMenuItemClickDetector = std::static_pointer_cast<ClickDetectorComponent>(
            screenBlinkingSceneMenuItemGameObject->findComponent(ClickDetectorComponent::TYPE_NAME)
    );
    throwErrorIfNull(
            m_screenBlinkingSceneMenuItemClickDetector,
            "Click detector for Screen Blinking Scene menu item not found"
    );
}
