//
// Created by Igor Lapin on 29/07/2020.
//

#include <engine_3d/Utils.h>
#include <main/L.h>
#include "ScenesSelectionScene.h"
#include "LoadingScene.h"

using namespace Engine3D::Utils;

void ScenesSelectionScene::update() {
    Scene::update();

    if (m_renderingEngineDevSceneMenuItemClickDetector->isClickDetected()) {
        std::unordered_map<std::string, RequestedSceneArgValue> args;
        args.insert({ LoadingScene::LOADING_SCENE_TYPE_KEY, static_cast<int>(SceneType::RENDERING_ENGINE_DEV_SCENE) });
        m_sceneManager->requestSceneLoadAndStart(SceneType::LOADING_SCENE, args);
    }

    if (m_screenBlinkingSceneMenuItemClickDetector->isClickDetected()) {
        m_sceneManager->requestSceneLoadAndStart(SceneType::SCREEN_BLINKING_SCENE);
    }

    if (m_multitouchTestSceneMenuItemClickDetector->isClickDetected()) {
        m_sceneManager->requestSceneLoadAndStart(SceneType::MULTITOUCH_TEST_SCENE);
    }
}

void ScenesSelectionScene::buildHierarchyFromRepresentation(const std::string& hierarchyRepresentation) {
    Scene::buildHierarchyFromRepresentation(hierarchyRepresentation);

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
