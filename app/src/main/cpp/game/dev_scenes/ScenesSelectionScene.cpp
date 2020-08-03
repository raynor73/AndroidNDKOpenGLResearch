//
// Created by Igor Lapin on 29/07/2020.
//

#include <engine_3d/Utils.h>
#include <main/L.h>
#include "ScenesSelectionScene.h"

using namespace Engine3D::Utils;

void ScenesSelectionScene::update(float) {
    if (m_renderingEngineDevSceneMenuItemClickDetector->isClickDetected()) {
        m_sceneManager->requestSceneLoadAndStart(SceneType::RENDERING_ENGINE_DEV_SCENE);
    }

    if (m_screenBlinkingSceneMenuItemClickDetector->isClickDetected()) {
        m_sceneManager->requestSceneLoadAndStart(SceneType::SCREEN_BLINKING_SCENE);
    }

    if (m_multitouchTestSceneMenuItemClickDetector->isClickDetected()) {
        m_sceneManager->requestSceneLoadAndStart(SceneType::MULTITOUCH_TEST_SCENE);
    }
}

void ScenesSelectionScene::restoreFromStateRepresentation(const std::string stateRepresentation) {
    Scene::restoreFromStateRepresentation(stateRepresentation);

    auto renderingEngineDevSceneMenuItemGameObject = m_gameObjectsMap.at("renderingEngineDevSceneMenuItem");
    auto screenBlinkingSceneMenuItemGameObject = m_gameObjectsMap.at("screenBlinkingSceneMenuItem");
    auto multitouchTestSceneMenuItemGameObject = m_gameObjectsMap.at("multitouchTestSceneMenuItem");

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

    m_multitouchTestSceneMenuItemClickDetector = std::static_pointer_cast<ClickDetectorComponent>(
            multitouchTestSceneMenuItemGameObject->findComponent(ClickDetectorComponent::TYPE_NAME)
    );
    throwErrorIfNull(
            m_multitouchTestSceneMenuItemClickDetector,
            "Click detector for Multitouch Test Scene menu item not found"
    );
}
