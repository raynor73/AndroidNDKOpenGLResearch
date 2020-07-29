//
// Created by Igor Lapin on 03/06/2020.
//

#include <game/dev_scenes/RenderingEngineDevScene.h>
#include <game/dev_scenes/ScenesSelectionScene.h>
#include "GameWrapper.h"
#include <game/SceneManager.h>

const std::string GameWrapper::TOUCH_EVENT_MESSAGE_TYPE_NAME = "TouchEvent";

GameWrapper::GameWrapper(
        float displayDensityFactor,
        JavaVM* javaVm,
        jclass bridgeClass,
        jobject bridgeObject
) : m_messageQueue(std::make_shared<MessageQueue::Queue>()),
    m_displayDensityFactor(displayDensityFactor),
    m_javaVm(javaVm),
    m_bridgeClass(bridgeClass),
    m_bridgeObject(bridgeObject),
    m_openGlErrorDetector(std::make_shared<OpenGLErrorDetector>()),
    m_sceneDataLoader(std::make_shared<AndroidSceneDataLoader>(
            m_javaVm,
            bridgeClass,
            bridgeObject
    )),
    m_meshLoadingRepository(std::make_shared<AndroidMeshLoadingRepository>(
            m_javaVm,
            bridgeClass,
            bridgeObject
    )),
    m_fontDataLoader(std::make_shared<AndroidFontDataLoader>(
            m_javaVm,
            bridgeClass,
            bridgeObject
    )),
    m_shadersRepository(std::make_shared<OpenGlShadersRepository>(m_openGlErrorDetector)),
    m_shaderSourceRepository(std::make_shared<AndroidShaderSourceRepository>(
            m_javaVm,
            bridgeClass,
            bridgeObject
    )),
    m_shaderSourcePreprocessor(std::make_shared<ShaderSourcePreprocessor>(m_shaderSourceRepository)),
    m_geometryBuffersStorage(std::make_shared<OpenGLGeometryBuffersStorage>(m_openGlErrorDetector)),
    m_verticalQuadBuffersRepository(std::make_shared<OpenGLVerticalQuadBuffersRepository>(m_geometryBuffersStorage)),
    m_texturesRepository(std::make_shared<OpenGLTexturesRepository>(m_openGlErrorDetector)),
    m_charactersRepository(std::make_shared<OpenGLFreeTypeCharactersRepository>(m_fontDataLoader, m_texturesRepository)),
    m_touchScreen(std::make_shared<AndroidTouchScreen>(m_messageQueue))
    {}

void GameWrapper::putTouchEventIntoQueue(std::shared_ptr<TouchEvent> touchEvent) {
    m_messageQueue->putMessage({ TOUCH_EVENT_MESSAGE_TYPE_NAME, touchEvent });
}

void GameWrapper::onDrawFrame() {
    if (m_requestedSceneTypeOptional) {
        m_texturesRepository->removeAllTextures();
        m_geometryBuffersStorage->removeAllBuffers();

        switch (m_requestedSceneTypeOptional.value()) {

            case SceneType::SCENES_SELECTION_SCENE:
                m_scene = std::make_shared<ScenesSelectionScene>(
                        std::make_shared<TimeProvider>(),
                        m_displayInfo,
                        m_unitsConverter,
                        m_meshLoadingRepository,
                        m_meshRendererFactory,
                        m_textRendererFactory,
                        m_touchScreen
                );
                m_sceneDataLoader->loadSceneData("scenes/scenes_selection_scene.json", *m_scene);
                break;

            case SceneType::RENDERING_ENGINE_DEV_SCENE:
                m_scene = std::make_shared<RenderingEngineDevScene>(
                        std::make_shared<TimeProvider>(),
                        m_displayInfo,
                        m_unitsConverter,
                        m_meshLoadingRepository,
                        m_meshRendererFactory,
                        m_textRendererFactory,
                        m_touchScreen
                );
                m_sceneDataLoader->loadSceneData("scenes/rendering_engine_dev_scene.json", *m_scene);
                break;

            case SceneType::SCREEN_BLINKING_SCENE:
                break;
        }
        m_requestedSceneTypeOptional.reset();
    }

    m_touchScreen->update();
    m_messageQueue->update();
    if (m_scene != nullptr) {
        m_scene->update();
        m_renderingEngine->render(*m_scene);
    }
}

void GameWrapper::onSurfaceChanged(int width, int height) {
    if (m_displayInfo == nullptr) {
        m_displayInfo = std::make_shared<AndroidDisplayInfo>(width, height, m_displayDensityFactor);
        m_unitsConverter = std::make_shared<AndroidUnitsConverter>(m_displayInfo);
    }
    m_displayInfo->setWidth(width);
    m_displayInfo->setHeight(height);

    if (m_renderingEngine == nullptr) {
        m_renderingEngine = std::make_shared<RenderingEngine>(
                m_openGlErrorDetector,
                m_unitsConverter,
                m_shadersRepository,
                m_shaderSourcePreprocessor,
                m_geometryBuffersStorage,
                m_texturesRepository
        );
        m_meshRendererFactory = std::make_shared<OpenGlMeshRendererFactory>(
                m_geometryBuffersStorage,
                m_openGlErrorDetector
        );
        m_textRendererFactory = std::make_shared<OpenGLTextRendererFactory>(
                m_openGlErrorDetector,
                m_verticalQuadBuffersRepository,
                m_charactersRepository,
                m_texturesRepository
        );
    } else {
        m_renderingEngine->onOpenGlContextRecreated();
    }
}

void GameWrapper::onSurfaceCreated() {
    // do nothing
}

void GameWrapper::requestSceneLoadAndStart(SceneType type) {
    m_requestedSceneTypeOptional = type;
}

