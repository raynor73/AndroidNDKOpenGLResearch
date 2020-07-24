//
// Created by Igor Lapin on 03/06/2020.
//

#include <game/dev_scenes/RenderingEngineDevScene.h>
#include "GameWrapper.h"

GameWrapper::GameWrapper(
        float displayDensityFactor,
        JavaVM* javaVm,
        jclass bridgeClass,
        jobject bridgeObject
) : m_displayDensityFactor(displayDensityFactor),
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
    m_charactersRepository(std::make_shared<OpenGLFreeTypeCharactersRepository>(m_fontDataLoader, m_texturesRepository))
    {}

void GameWrapper::onDrawFrame() {
    m_messageQueue.update();
    m_scene->update();
    m_renderingEngine->render(*m_scene);
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

    if (m_scene == nullptr) {
        m_scene = std::make_shared<RenderingEngineDevScene>(
                std::make_shared<TimeProvider>(),
                m_displayInfo,
                m_unitsConverter,
                m_meshLoadingRepository,
                m_meshRendererFactory,
                m_textRendererFactory
        );
        m_sceneDataLoader->loadSceneData("scenes/rendering_engine_dev_scene.json", *m_scene);
    }
}

void GameWrapper::onSurfaceCreated() {
    // do nothing
}

