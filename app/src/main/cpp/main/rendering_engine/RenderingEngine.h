//
// Created by Igor Lapin on 28/06/2020.
//

#ifndef GAME_RENDERINGENGINE_H
#define GAME_RENDERINGENGINE_H


#include <stack>
#include <game/Scene.h>
#include <main/OpenGLErrorDetector.h>
#include <game/UnitsConverter.h>
#include <engine_3d/CameraComponent.h>
#include <engine_3d/AmbientLightComponent.h>
#include "OpenGlMeshRendererComponent.h"
#include "OpenGLFreeTypeTextRendererComponent.h"
#include "OpenGLState.h"
#include "OpenGlShadersRepository.h"
#include "ShaderSourcePreprocessor.h"
#include "OpenGLGeometryBuffersStorage.h"
#include "OpenGLTexturesRepository.h"

class RenderingEngine {

    std::stack<OpenGLState> m_openGLStateStack;
    std::shared_ptr<OpenGLErrorDetector> m_openGLErrorDetector;
    std::shared_ptr<UnitsConverter> m_unitsConverter;
    std::shared_ptr<OpenGlShadersRepository> m_shadersRepository;
    std::shared_ptr<ShaderSourcePreprocessor> m_shaderSourcePreprocessor;
    std::shared_ptr<OpenGLGeometryBuffersStorage> m_geometryBuffersStorage;
    std::shared_ptr<OpenGLTexturesRepository> m_texturesRepository;

public:
    RenderingEngine(
            std::shared_ptr<OpenGLErrorDetector> openGLErrorDetector,
            std::shared_ptr<UnitsConverter> unitsConverter,
            std::shared_ptr<OpenGlShadersRepository> shadersRepository,
            std::shared_ptr<ShaderSourcePreprocessor> shaderSourcePreprocessor,
            std::shared_ptr<OpenGLGeometryBuffersStorage> geometryBuffersStorage,
            std::shared_ptr<OpenGLTexturesRepository> texturesRepository
    );
    RenderingEngine(const RenderingEngine&) = delete;
    RenderingEngine(RenderingEngine&&) = delete;

    void render(Scene& scene);
    void onOpenGlContextRecreated();

    RenderingEngine& operator=(const RenderingEngine&) = delete;
    RenderingEngine& operator=(RenderingEngine&&) = delete;

private:
    bool m_isErrorLogged;

    static void traverseSceneHierarchy(GameObject& gameObject, const std::function<void(GameObject&)>& callback);

    void putMeshInGeometryBuffersIfNecessary(const std::string& name, const Mesh& mesh);

    void renderMeshWithAllRequiredShaders(
            const std::shared_ptr<CameraComponent>& camera,
            const std::shared_ptr<OpenGlMeshRendererComponent>& meshRenderer,
            const std::unordered_map<std::string, std::shared_ptr<AmbientLightComponent>>& layerNameToAmbientLightMap,
            const std::string& layerName
    );
    static void renderMesh(
            const std::shared_ptr<CameraComponent>& camera,
            const std::shared_ptr<OpenGlMeshRendererComponent>& meshRenderer,
            ShaderType shaderType,
            const OpenGlShaderProgramContainer& shaderProgramContainer
    );
    void renderText(
            const std::shared_ptr<CameraComponent>& camera,
            const std::shared_ptr<OpenGLFreeTypeTextRendererComponent>& textRenderer
    );

    void pushOpenGLState(const OpenGLState& state);
    void popOpenGLState();
    void applyOpenGLState(const OpenGLState& state);
};


#endif //GAME_RENDERINGENGINE_H
