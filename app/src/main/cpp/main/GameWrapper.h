//
// Created by Igor Lapin on 03/06/2020.
//

#ifndef ANDROID_NDK_OPENGL_RESEARCH_GAMEWRAPPER_H
#define ANDROID_NDK_OPENGL_RESEARCH_GAMEWRAPPER_H


#include <memory>
#include <jni.h>
#include <optional>
#include <game/MessageQueue.h>
#include <main/rendering_engine/RenderingEngine.h>
#include <main/rendering_engine/OpenGLGeometryBuffersStorage.h>
#include <main/AndroidSceneDataLoader.h>
#include <main/AndroidUnitsConverter.h>
#include <main/AndroidMeshLoadingRepository.h>
#include <main/rendering_engine/OpenGlMeshRendererFactory.h>
#include <main/rendering_engine/OpenGLTextRendererFactory.h>
#include <main/OpenGLErrorDetector.h>
#include <main/rendering_engine/OpenGLVerticalQuadBuffersRepository.h>
#include <main/rendering_engine/OpenGLFreeTypeCharactersRepository.h>
#include <main/rendering_engine/OpenGLTexturesRepository.h>
#include <game/touch_screen/TouchEvent.h>
#include <game/SceneManager.h>
#include <game/TimeProvider.h>
#include <engine_3d/BulletPhysicsEngine.h>
#include <main/AndroidSkeletalAnimationLoadingRepository.h>
#include <engine_3d/AppStateRepository.h>
#include "AndroidBitmapDataLoader.h"
#include "AndroidDisplayInfo.h"
#include "AndroidShaderSourceRepository.h"
#include "AndroidFontDataLoader.h"
#include "AndroidTouchScreen.h"
#include "AndroidFsAbstraction.h"
#include "AndroidSoundLoadingRepository.h"
#include "AndroidOpenALSoundScene.h"

class GameWrapper : public SceneManager, public AppStateRepository {

    std::shared_ptr<MessageQueue::Queue> m_messageQueue;
    float m_displayDensityFactor;
    JavaVM* m_javaVm;
    jclass m_bridgeClass;
    jobject m_bridgeObject;
    std::shared_ptr<OpenGLErrorDetector> m_openGlErrorDetector;
    std::shared_ptr<TimeProvider> m_timeProvider;
    std::shared_ptr<Time> m_time;
    std::shared_ptr<AndroidDisplayInfo> m_displayInfo;
    std::shared_ptr<AndroidUnitsConverter> m_unitsConverter;
    std::shared_ptr<Scene> m_scene;
    std::shared_ptr<RenderingEngine> m_renderingEngine;
    std::shared_ptr<AndroidSceneDataLoader> m_sceneDataLoader;
    std::shared_ptr<AndroidMeshLoadingRepository> m_meshLoadingRepository;
    std::shared_ptr<AndroidFontDataLoader> m_fontDataLoader;
    std::shared_ptr<OpenGlMeshRendererFactory> m_meshRendererFactory;
    std::shared_ptr<OpenGLTextRendererFactory> m_textRendererFactory;
    std::shared_ptr<OpenGlShadersRepository> m_shadersRepository;
    std::shared_ptr<AndroidShaderSourceRepository> m_shaderSourceRepository;
    std::shared_ptr<ShaderSourcePreprocessor> m_shaderSourcePreprocessor;
    std::shared_ptr<OpenGLGeometryBuffersStorage> m_geometryBuffersStorage;
    std::shared_ptr<OpenGLVerticalQuadBuffersRepository> m_verticalQuadBuffersRepository;
    std::shared_ptr<AndroidBitmapDataLoader> m_bitmapDataLoader;
    std::shared_ptr<OpenGLTexturesRepository> m_texturesRepository;
    std::shared_ptr<OpenGLFreeTypeCharactersRepository> m_charactersRepository;
    std::shared_ptr<AndroidTouchScreen> m_touchScreen;
    std::shared_ptr<SceneManager> m_sceneManager;
    std::shared_ptr<AppStateRepository> m_appStateRepository;
    std::shared_ptr<BulletPhysicsEngine> m_physicsEngine;
    std::shared_ptr<AndroidSkeletalAnimationLoadingRepository> m_skeletalAnimationsRepository;
    std::shared_ptr<AndroidFsAbstraction> m_fsAbstraction;
    std::shared_ptr<ReadOnlyFsAbstraction> m_readOnlyFsAbstraction;
    std::shared_ptr<AndroidSoundLoadingRepository> m_soundLoadingRepository;
    std::shared_ptr<SoundStorage> m_soundStorage;
    std::shared_ptr<AndroidOpenALSoundScene> m_soundScene;

    std::optional<SceneType> m_requestedSceneTypeOptional;
    std::unordered_map<std::string, RequestedSceneArgValue> m_requestedSceneArgs;

    bool m_isAppInForeground = true;

public:
    explicit GameWrapper(
            float displayDensityFactor,
            JavaVM* javaVm,
            jclass bridgeClass,
            jobject bridgeObject
    );
    GameWrapper(const GameWrapper&) = delete;
    GameWrapper(GameWrapper&&) = delete;

    void putTouchEventIntoQueue(std::shared_ptr<TouchEvent> touchEvent);
    void onDrawFrame();
    void onSurfaceChanged(int width, int height);
    void onSurfaceCreated();

    virtual void requestSceneLoadAndStart(
            SceneType type,
            std::unordered_map<std::string, RequestedSceneArgValue> args
    ) override;

    void requestSceneLoadAndStart(SceneType type) override;

    void setAppInForeground(bool isAppInForeground) { m_isAppInForeground = isAppInForeground; }
    virtual bool isAppInForeground() const override { return m_isAppInForeground; }

    GameWrapper& operator=(const GameWrapper&) = delete;
    GameWrapper& operator=(GameWrapper&&) = delete;

    static const std::string TOUCH_EVENT_MESSAGE_TYPE_NAME;
};


#endif //ANDROID_NDK_OPENGL_RESEARCH_GAMEWRAPPER_H
