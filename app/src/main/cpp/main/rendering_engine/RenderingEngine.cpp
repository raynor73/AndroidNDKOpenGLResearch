//
// Created by Igor Lapin on 28/06/2020.
//

#include <vector>
#include <glm/gtx/compatibility.hpp>
#include <engine_3d/CameraComponent.h>
#include <engine_3d/OrthoCameraComponent.h>
#include <main/AndroidMeshRendererComponent.h>
#include <unordered_map>
#include <main/L.h>
#include <main/Constants.h>
#include "RenderingEngine.h"

RenderingEngine::RenderingEngine(
        std::shared_ptr<OpenGLErrorDetector> openGLErrorDetector,
        std::shared_ptr<UnitsConverter> unitsConverter,
        std::shared_ptr<OpenGlShadersRepository> shadersRepository,
        std::shared_ptr<ShaderSourcePreprocessor> shaderSourcePreprocessor,
        std::shared_ptr<OpenGLGeometryBuffersStorage> m_geometryBuffersStorage
) : m_openGLErrorDetector(openGLErrorDetector),
    m_unitsConverter(unitsConverter),
    m_shadersRepository(shadersRepository),
    m_geometryBuffersStorage(m_geometryBuffersStorage),
    m_isErrorLogged(false)
{
    auto unlitVertexShaderSource = shaderSourcePreprocessor->loadShaderSource(
            "shaders/unlit/unlitVertexShader.glsl"
    );
    auto unlitFragmentShaderSource = shaderSourcePreprocessor->loadShaderSource(
            "shaders/unlit/unlitFragmentShader.glsl"
    );
    shadersRepository->createVertexShader("unlit", unlitVertexShaderSource);
    shadersRepository->createFragmentShader("unlit", unlitFragmentShaderSource);
    shadersRepository->createShaderProgram("unlit", "unlit", "unlit");
}

void RenderingEngine::render(Scene &scene) {
    if (m_openGLErrorDetector->isOpenGLErrorDetected()) {
        if (!m_isErrorLogged) {
            m_isErrorLogged = true;
            L::e(
                    App::Constants::LOG_TAG,
                    "Rendering Engine will not render anymore as OpenGL error have been detected"
            );
        }

        return;
    }

    std::vector<std::shared_ptr<CameraComponent>> activeCameras;
    std::unordered_multimap<std::string, std::shared_ptr<AndroidMeshRendererComponent>> layerNameToMeshRendererMap;

    traverseSceneHierarchy(*scene.rootGameObject(), [&](GameObject& gameObject) {
        if (auto camera = gameObject.findComponent(OrthoCameraComponent::TYPE_NAME); camera != nullptr) {
            if (camera->isEnabled()) {
                activeCameras.push_back(std::static_pointer_cast<CameraComponent>(camera));
            }
        }

        if (
                auto meshRenderer = std::static_pointer_cast<AndroidMeshRendererComponent>(
                        gameObject.findComponent(AndroidMeshRendererComponent::TYPE_NAME)
                );
                meshRenderer != nullptr
        ) {
            for (auto& layerName : meshRenderer->layerNames()) {
                layerNameToMeshRendererMap.insert({layerName, meshRenderer });
            }
        }

        /*if (auto camera = gameObject.findComponent(PerspectiveCameraComponent::TYPE_NAME); camera != nullptr) {

        }*/
    });

    for (auto& camera : activeCameras) {
        GLbitfield clearMask = 0;
        if (camera->shouldClearColor()) {
            clearMask |= GL_COLOR_BUFFER_BIT;
        }
        if (camera->shouldClearDepth()) {
            clearMask |= GL_DEPTH_BUFFER_BIT;
        }

        int viewportX = m_unitsConverter->widthPercentToPixels(camera->viewportX() * 100);
        int viewportY =  m_unitsConverter->heightPercentToPixels(camera->viewportY() * 100);
        int viewportWidth = m_unitsConverter->widthPercentToPixels(camera->viewportWidth() * 100);
        int viewportHeight = m_unitsConverter->heightPercentToPixels(camera->viewportHeight() * 100);
        OpenGLState openGlState {
            {viewportX, viewportY, viewportWidth, viewportHeight},
            {viewportX, viewportY, viewportWidth, viewportHeight},
            false,
            {GL_ONE, GL_ONE},
            true,
            GL_LESS
        };
        pushOpenGLState(openGlState);

        auto clearColor = camera->clearColor();
        glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
        glClear(clearMask);

        for (auto& layerName : camera->layerNames()) {
            for (
                    auto it = layerNameToMeshRendererMap.find(layerName);
                    it != layerNameToMeshRendererMap.end();
                    it++
            ) {
                it->second->render();
            }
        }

        popOpenGLState();
    }
}

void RenderingEngine::onOpenGlContextRecreated() {
    m_shadersRepository->restoreShaders();
}

void RenderingEngine::traverseSceneHierarchy(GameObject &gameObject, const std::function<void(GameObject &)>& callback) {
    callback(gameObject);
    for (auto& entry : gameObject.children()) {
        traverseSceneHierarchy(*entry.second, callback);
    }
}

void RenderingEngine::pushOpenGLState(const OpenGLState& state) {
    applyOpenGLState(state);
    m_openGLStateStack.push(state);
}

void RenderingEngine::popOpenGLState() {
    m_openGLStateStack.pop();
    if (!m_openGLStateStack.empty()) {
        applyOpenGLState(m_openGLStateStack.top());
    }
}

void RenderingEngine::applyOpenGLState(const OpenGLState& state) {
    glViewport(state.viewport.x, state.viewport.y, state.viewport.width, state.viewport.height);
    glScissor(state.scissor.x, state.scissor.y, state.scissor.width, state.scissor.height);
    if (state.blend) {
        glEnable(GL_BLEND);
    } else {
        glDisable(GL_BLEND);
    }
    glBlendFunc(state.blendFunction.sFactor, state.blendFunction.dFactor);
    glDepthMask(state.depthMask);
    glDepthFunc(state.depthFunction);

    m_openGLErrorDetector->checkOpenGLErrors("RenderingEngine::applyOpenGLState");
}

void RenderingEngine::checkMeshInGeometryBuffersAndCreateIfNecessary(const std::string& name, const Mesh& mesh) {
    std::vector<float> vertexData;
    m_geometryBuffersStorage->createStaticVertexBuffer(name, vertexData);
}
