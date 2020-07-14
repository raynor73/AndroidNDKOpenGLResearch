//
// Created by Igor Lapin on 28/06/2020.
//

#ifndef GAME_RENDERINGENGINE_H
#define GAME_RENDERINGENGINE_H


#include <stack>
#include <game/Scene.h>
#include <main/OpenGLErrorDetector.h>
#include <game/UnitsConverter.h>
#include "OpenGLState.h"
#include "OpenGlShadersRepository.h"
#include "ShaderSourcePreprocessor.h"
#include "OpenGLGeometryBuffersStorage.h"

class RenderingEngine {

    std::stack<OpenGLState> m_openGLStateStack;
    std::shared_ptr<OpenGLErrorDetector> m_openGLErrorDetector;
    std::shared_ptr<UnitsConverter> m_unitsConverter;
    std::shared_ptr<OpenGlShadersRepository> m_shadersRepository;
    std::shared_ptr<ShaderSourcePreprocessor> m_shaderSourcePreprocessor;
    std::shared_ptr<OpenGLGeometryBuffersStorage> m_geometryBuffersStorage;

public:
    RenderingEngine(
            std::shared_ptr<OpenGLErrorDetector> openGLErrorDetector,
            std::shared_ptr<UnitsConverter> unitsConverter,
            std::shared_ptr<OpenGlShadersRepository> shadersRepository,
            std::shared_ptr<ShaderSourcePreprocessor> shaderSourcePreprocessor,
            std::shared_ptr<OpenGLGeometryBuffersStorage> m_geometryBuffersStorage
    );
    RenderingEngine(const RenderingEngine&) = delete;
    RenderingEngine(RenderingEngine&&) = delete;

    void render(Scene& scene);
    void onOpenGlContextRecreated();

    RenderingEngine& operator=(const RenderingEngine&) = delete;
    RenderingEngine& operator=(RenderingEngine&&) = delete;

private:
    bool m_isErrorLogged;

    void traverseSceneHierarchy(GameObject& gameObject, const std::function<void(GameObject&)>& callback);

    void putMeshInGeometryBuffersIfNecessary(const std::string& name, const Mesh& mesh);

    void pushOpenGLState(const OpenGLState& state);
    void popOpenGLState();
    void applyOpenGLState(const OpenGLState& state);
};


#endif //GAME_RENDERINGENGINE_H
