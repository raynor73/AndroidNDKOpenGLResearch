//
// Created by Igor Lapin on 03/06/2020.
//

#include <game/dev_scenes/RenderingEngineDevScene.h>
#include <game/dev_scenes/ScenesSelectionScene.h>
#include "GameWrapper.h"
#include "AndroidReadOnlyFsAbstraction.h"
#include <game/SceneManager.h>
#include <game/dev_scenes/ScreenBlinkingScene.h>
#include <game/dev_scenes/MultitouchTestScene.h>
#include <game/dev_scenes/LoadingScene.h>

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
    m_timeProvider(std::make_shared<TimeProvider>()),
    m_time(std::make_shared<Time>(m_timeProvider)),
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
    m_bitmapDataLoader(std::make_shared<AndroidBitmapDataLoader>(
            m_javaVm,
            bridgeClass,
            bridgeObject
    )),
    m_touchScreen(std::make_shared<AndroidTouchScreen>(m_messageQueue)),
    m_physicsEngine(std::make_shared<BulletPhysicsEngine>()),
    m_skeletalAnimationsRepository(std::make_shared<AndroidSkeletalAnimationLoadingRepository>(
            m_javaVm,
            bridgeClass,
            bridgeObject
    )),
    m_fsAbstraction(std::make_shared<AndroidFsAbstraction>(m_javaVm, bridgeClass, bridgeObject)),
    m_readOnlyFsAbstraction(std::make_shared<AndroidReadOnlyFsAbstraction>(m_javaVm, bridgeClass, bridgeObject)),
    m_soundLoadingRepository(std::make_shared<AndroidSoundLoadingRepository>(m_readOnlyFsAbstraction)),
    m_soundStorage(std::make_shared<SoundStorage>()),
    m_soundScene(std::make_shared<AndroidOpenALSoundScene>(m_soundStorage))
    {
        // TODO Make another solution instead of this. Despite in current case this works fine in future it can lead to crashes because of multiple delete calls or similar issues.
        auto gameWrapperSharedPtr = std::shared_ptr<GameWrapper>(this);
        m_sceneManager = gameWrapperSharedPtr;
        m_appStateRepository = gameWrapperSharedPtr;
    }

void GameWrapper::putTouchEventIntoQueue(std::shared_ptr<TouchEvent> touchEvent) {
    m_messageQueue->putMessage({ TOUCH_EVENT_MESSAGE_TYPE_NAME, touchEvent });
}

void GameWrapper::onDrawFrame() {
    if (m_requestedSceneTypeOptional) {
        m_textRendererFactory->removeAllCharacters();
        m_texturesRepository->removeAllTextures();
        m_geometryBuffersStorage->removeAllBuffers();
        m_soundStorage->removeAllSounds();

        m_scene.reset();
        m_physicsEngine->reset();

        switch (m_requestedSceneTypeOptional.value()) {

            case SceneType::SCENES_SELECTION_SCENE:
                m_scene = std::make_shared<ScenesSelectionScene>(
                        m_time,
                        m_displayInfo,
                        m_unitsConverter,
                        m_meshLoadingRepository,
                        m_meshRendererFactory,
                        m_textRendererFactory,
                        m_touchScreen,
                        m_sceneManager,
                        m_texturesRepository,
                        m_physicsEngine,
                        m_skeletalAnimationsRepository,
                        m_soundLoadingRepository,
                        m_soundStorage,
                        m_soundScene,
                        m_appStateRepository
                );
                m_sceneDataLoader->loadSceneData("scenes/scenes_selection_scene.json", *m_scene);
                break;

            case SceneType::RENDERING_ENGINE_DEV_SCENE:
                m_scene = std::make_shared<RenderingEngineDevScene>(
                        m_time,
                        m_displayInfo,
                        m_unitsConverter,
                        m_meshLoadingRepository,
                        m_meshRendererFactory,
                        m_textRendererFactory,
                        m_touchScreen,
                        m_texturesRepository,
                        m_sceneManager,
                        m_physicsEngine,
                        m_skeletalAnimationsRepository,
                        m_soundLoadingRepository,
                        m_fsAbstraction,
                        m_soundStorage,
                        m_soundScene,
                        m_appStateRepository
                );
                m_sceneDataLoader->loadSceneData("scenes/rendering_engine_dev_scene.json", *m_scene);
                break;

            case SceneType::SCREEN_BLINKING_SCENE:
                m_scene = std::make_shared<ScreenBlinkingScene>(
                        m_time,
                        m_displayInfo,
                        m_unitsConverter,
                        m_meshLoadingRepository,
                        m_meshRendererFactory,
                        m_textRendererFactory,
                        m_touchScreen,
                        m_texturesRepository,
                        m_physicsEngine,
                        m_skeletalAnimationsRepository,
                        m_soundLoadingRepository,
                        m_soundStorage,
                        m_soundScene,
                        m_appStateRepository
                );
                break;

            case SceneType::MULTITOUCH_TEST_SCENE:
                m_scene = std::make_shared<MultitouchTestScene>(
                        m_time,
                        m_displayInfo,
                        m_unitsConverter,
                        m_meshLoadingRepository,
                        m_meshRendererFactory,
                        m_textRendererFactory,
                        m_touchScreen,
                        m_texturesRepository,
                        m_sceneManager,
                        m_physicsEngine,
                        m_skeletalAnimationsRepository,
                        m_soundLoadingRepository,
                        m_soundStorage,
                        m_soundScene,
                        m_appStateRepository
                );
                m_sceneDataLoader->loadSceneData("scenes/multitouch_test_scene.json", *m_scene);
                break;

            case SceneType::LOADING_SCENE:
                m_scene = std::make_shared<LoadingScene>(
                        m_requestedSceneArgs,
                        m_time,
                        m_displayInfo,
                        m_unitsConverter,
                        m_meshLoadingRepository,
                        m_meshRendererFactory,
                        m_textRendererFactory,
                        m_touchScreen,
                        m_texturesRepository,
                        m_sceneManager,
                        m_physicsEngine,
                        m_skeletalAnimationsRepository,
                        m_soundLoadingRepository,
                        m_soundStorage,
                        m_soundScene,
                        m_appStateRepository
                );
                m_sceneDataLoader->loadSceneData("scenes/loading_scene.json", *m_scene);
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
        m_texturesRepository = std::make_shared<OpenGLTexturesRepository>(
                m_displayInfo,
                m_bitmapDataLoader,
                m_openGlErrorDetector
        );
        m_charactersRepository = std::make_shared<OpenGLFreeTypeCharactersRepository>(
                m_fontDataLoader,
                m_texturesRepository,
                m_unitsConverter,
                m_displayInfo
        );
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
                m_texturesRepository,
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

void GameWrapper::requestSceneLoadAndStart(
        SceneType type,
        std::unordered_map<std::string, RequestedSceneArgValue> args
) {
    m_requestedSceneTypeOptional = type;
    m_requestedSceneArgs = std::move(args);
}

void GameWrapper::requestSceneLoadAndStart(SceneType type) {
    requestSceneLoadAndStart(type, {});
}
