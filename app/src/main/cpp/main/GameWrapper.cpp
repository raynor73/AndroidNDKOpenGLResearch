//
// Created by Igor Lapin on 03/06/2020.
//

#include <game/dev_scenes/RenderingEngineDevScene.h>
#include "GameWrapper.h"

GameWrapper::GameWrapper(
        float displayDensityFactor,
        JavaVM* javaVm,
        jclass sceneJsonStringLoaderClass,
        jobject sceneJsonStringLoaderObject
) : m_displayDensityFactor(displayDensityFactor),
    m_javaVm(javaVm),
    m_sceneJsonStringLoaderClass(sceneJsonStringLoaderClass),
    m_sceneJsonStringLoaderObject(sceneJsonStringLoaderObject),
    m_sceneDataLoader(std::make_shared<AndroidSceneDataLoader>(
            m_javaVm,
            sceneJsonStringLoaderClass,
            sceneJsonStringLoaderObject
    )) {}

void GameWrapper::onDrawFrame() {
    m_messageQueue.update();
    m_scene->update();
    m_renderingEngine->render(*m_scene);
}

void GameWrapper::onSurfaceChanged(int width, int height) {
    if (m_displayInfo == nullptr) {
        m_displayInfo = std::make_shared<AndroidDisplayInfo>(width, height, m_displayDensityFactor);
    }
    m_displayInfo->setWidth(width);
    m_displayInfo->setHeight(height);

    if (m_renderingEngine == nullptr) {
        m_renderingEngine = std::make_shared<RenderingEngine>(m_displayInfo);
    }

    if (m_scene == nullptr) {
        m_scene = std::make_shared<RenderingEngineDevScene>(
                std::make_shared<TimeProvider>(),
                m_displayInfo
        );
        m_sceneDataLoader->loadSceneData("scenes/rendering_engine_dev_scene.json", *m_scene);
    }
}

void GameWrapper::onSurfaceCreated() {
    // do nothing
}

