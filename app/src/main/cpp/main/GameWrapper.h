//
// Created by Igor Lapin on 03/06/2020.
//

#ifndef ANDROID_NDK_OPENGL_RESEARCH_GAMEWRAPPER_H
#define ANDROID_NDK_OPENGL_RESEARCH_GAMEWRAPPER_H


#include <memory>
#include <jni.h>
#include <game/MessageQueue.h>
#include <main/rendering_engine/RenderingEngine.h>
#include <main/AndroidSceneDataLoader.h>
#include <main/AndroidUnitsConverter.h>
#include <main/AndroidMeshLoadingRepository.h>
#include <main/AndroidMeshRendererFactory.h>
#include <main/OpenGLErrorDetector.h>
#include "AndroidDisplayInfo.h"
#include "AndroidShaderSourceRepository.h"

class GameWrapper {

    MessageQueue m_messageQueue;
    float m_displayDensityFactor;
    JavaVM* m_javaVm;
    jclass m_bridgeClass;
    jobject m_bridgeObject;
    std::shared_ptr<OpenGLErrorDetector> m_openGlErrorDetector;
    std::shared_ptr<AndroidDisplayInfo> m_displayInfo;
    std::shared_ptr<AndroidUnitsConverter> m_unitsConverter;
    std::shared_ptr<Scene> m_scene;
    std::shared_ptr<RenderingEngine> m_renderingEngine;
    std::shared_ptr<AndroidSceneDataLoader> m_sceneDataLoader;
    std::shared_ptr<AndroidMeshLoadingRepository> m_meshLoadingRepository;
    std::shared_ptr<AndroidMeshRendererFactory> m_meshRendererFactory;
    std::shared_ptr<OpenGlShadersRepository> m_shadersRepository;
    std::shared_ptr<AndroidShaderSourceRepository> m_shaderSourceRepository;
    std::shared_ptr<ShaderSourcePreprocessor> m_shaderSourcePreprocessor;

public:
    explicit GameWrapper(
            float displayDensityFactor,
            JavaVM* javaVm,
            jclass bridgeClass,
            jobject bridgeObject
    );
    GameWrapper(const GameWrapper&) = delete;
    GameWrapper(GameWrapper&&) = delete;

    void onDrawFrame();
    void onSurfaceChanged(int width, int height);
    void onSurfaceCreated();

    GameWrapper& operator=(const GameWrapper&) = delete;
    GameWrapper& operator=(GameWrapper&&) = delete;
};


#endif //ANDROID_NDK_OPENGL_RESEARCH_GAMEWRAPPER_H
