//
// Created by Igor Lapin on 28/06/2020.
//

#include <vector>
#include <algorithm>
#include <glm/gtx/compatibility.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/mat4x4.hpp>
#include <engine_3d/OrthoCameraComponent.h>
#include <engine_3d/MeshComponent.h>
#include <unordered_map>
#include <main/L.h>
#include <main/Constants.h>
#include <engine_3d/TransformationComponent.h>
#include <engine_3d/MaterialComponent.h>
#include <engine_3d/Utils.h>
#include <engine_3d/PerspectiveCameraComponent.h>
#include "RenderingEngine.h"

using namespace Engine3D::Utils;

RenderingEngine::RenderingEngine(
        std::shared_ptr<OpenGLErrorDetector> openGLErrorDetector,
        std::shared_ptr<UnitsConverter> unitsConverter,
        std::shared_ptr<OpenGlShadersRepository> shadersRepository,
        std::shared_ptr<ShaderSourcePreprocessor> shaderSourcePreprocessor,
        std::shared_ptr<OpenGLGeometryBuffersStorage> geometryBuffersStorage,
        std::shared_ptr<OpenGLTexturesRepository> texturesRepository
) : m_openGLErrorDetector(openGLErrorDetector),
    m_unitsConverter(unitsConverter),
    m_shadersRepository(shadersRepository),
    m_geometryBuffersStorage(geometryBuffersStorage),
    m_texturesRepository(texturesRepository),
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

    auto textVertexShaderSource = shaderSourcePreprocessor->loadShaderSource(
            "shaders/text/textVertexShader.glsl"
    );
    auto textFragmentShaderSource = shaderSourcePreprocessor->loadShaderSource(
            "shaders/text/textFragmentShader.glsl"
    );
    shadersRepository->createVertexShader("text", textVertexShaderSource);
    shadersRepository->createFragmentShader("text", textFragmentShaderSource);
    shadersRepository->createShaderProgram("text", "text", "text");

    auto ambientVertexShaderSource = shaderSourcePreprocessor->loadShaderSource(
            "shaders/light/ambientVertexShader.glsl"
    );
    auto ambientFragmentShaderSource = shaderSourcePreprocessor->loadShaderSource(
            "shaders/light/ambientFragmentShader.glsl"
    );
    shadersRepository->createVertexShader("ambient", ambientVertexShaderSource);
    shadersRepository->createFragmentShader("ambient", ambientFragmentShaderSource);
    shadersRepository->createShaderProgram("ambient", "ambient", "ambient");

    glEnable(GL_DEPTH_TEST);
    glFrontFace(GL_CCW);
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
    std::unordered_map<std::string, std::shared_ptr<AmbientLightComponent>> layerNameToAmbientLightMap;
    std::unordered_multimap<std::string, std::shared_ptr<OpenGlMeshRendererComponent>> layerNameToMeshRendererMap;
    std::unordered_multimap<std::string, std::shared_ptr<OpenGlMeshRendererComponent>> layerNameToTranslucentMeshRendererMap;
    std::unordered_multimap<std::string, std::shared_ptr<OpenGLFreeTypeTextRendererComponent>> layerNameToTextRendererMap;

    traverseSceneHierarchy(*scene.rootGameObject(), [&](GameObject& gameObject) {
        if (auto camera = gameObject.findComponent(OrthoCameraComponent::TYPE_NAME); camera != nullptr) {
            if (camera->isEnabled()) {
                activeCameras.push_back(std::static_pointer_cast<CameraComponent>(camera));
            }
        }

        if (auto camera = gameObject.findComponent(PerspectiveCameraComponent::TYPE_NAME); camera != nullptr) {
            if (camera->isEnabled()) {
                activeCameras.push_back(std::static_pointer_cast<CameraComponent>(camera));
            }
        }

        if (
                auto meshRenderer = std::static_pointer_cast<OpenGlMeshRendererComponent>(
                        gameObject.findComponent(OpenGlMeshRendererComponent::TYPE_NAME)
                );
                meshRenderer != nullptr
        ) {
            for (auto& layerName : meshRenderer->layerNames()) {
                auto materialComponent = std::static_pointer_cast<MaterialComponent>(
                        gameObject.findComponent(MaterialComponent::TYPE_NAME)
                );
                throwErrorIfNull(materialComponent, "Can't find material component for mesh renderer while rendering");
                if (materialComponent->material().isTranslucent) {
                    layerNameToTranslucentMeshRendererMap.insert({ layerName, meshRenderer });
                } else {
                    layerNameToMeshRendererMap.insert({ layerName, meshRenderer });
                }
            }
        }

        if (
                auto textRenderer = std::static_pointer_cast<OpenGLFreeTypeTextRendererComponent>(
                        gameObject.findComponent(OpenGLFreeTypeTextRendererComponent::TYPE_NAME)
                );
                textRenderer != nullptr
        ) {
            for (auto& layerName : textRenderer->layerNames()) {
                layerNameToTextRendererMap.insert({layerName, textRenderer });
            }
        }

        if (
                auto meshComponent = std::static_pointer_cast<MeshComponent>(
                        gameObject.findComponent(MeshComponent::TYPE_NAME)
                );
                meshComponent != nullptr
        ) {
            putMeshInGeometryBuffersIfNecessary(meshComponent->meshName(), meshComponent->mesh());
        }

        if (
                auto ambientLight = std::static_pointer_cast<AmbientLightComponent>(
                        gameObject.findComponent(AmbientLightComponent::TYPE_NAME)
                );
                ambientLight != nullptr
        ) {
            for (auto& layerName : ambientLight->layerNames()) {
                layerNameToAmbientLightMap.insert({ layerName, ambientLight });
            }
        }
    });

    std::sort(
            activeCameras.begin(),
            activeCameras.end(),
            [](const std::shared_ptr<CameraComponent>& lhs, const std::shared_ptr<CameraComponent>& rhs) {
                return lhs->order() < rhs->order();
            }
    );

    for (auto& camera : activeCameras) {
        GLbitfield clearMask = 0;
        if (camera->shouldClearColor()) {
            clearMask |= GL_COLOR_BUFFER_BIT;
        }
        if (camera->shouldClearDepth()) {
            clearMask |= GL_DEPTH_BUFFER_BIT;
        }

        int viewportX = m_unitsConverter->complexValueToPixels(camera->viewportX());
        int viewportY =  m_unitsConverter->complexValueToPixels(camera->viewportY());
        int viewportWidth = m_unitsConverter->complexValueToPixels(camera->viewportWidth());
        int viewportHeight = m_unitsConverter->complexValueToPixels(camera->viewportHeight());
        pushOpenGLState({
            { viewportX, viewportY, viewportWidth, viewportHeight },
            { viewportX, viewportY, viewportWidth, viewportHeight },
            false,
            {GL_ONE, GL_ONE},
            true,
            GL_LESS
        });

        auto clearColor = camera->clearColor();
        glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
        glClear(clearMask);

        for (auto& layerName : camera->layerNames()) {
            for (
                    auto it = layerNameToMeshRendererMap.find(layerName);
                    it != layerNameToMeshRendererMap.end();
                    it++
            ) {
                renderMeshWithAllRequiredShaders(camera, it->second, layerNameToAmbientLightMap, layerName);
            }

            pushOpenGLState({
                {viewportX, viewportY, viewportWidth, viewportHeight},
                {viewportX, viewportY, viewportWidth, viewportHeight},
                true,
                {GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA},
                true,
                GL_LESS
            });

            for (
                    auto it = layerNameToTranslucentMeshRendererMap.find(layerName);
                    it != layerNameToMeshRendererMap.end();
                    it++
            ) {
                renderMeshWithAllRequiredShaders(camera, it->second, layerNameToAmbientLightMap, layerName);
            }

            for (
                    auto it = layerNameToTextRendererMap.find(layerName);
                    it != layerNameToTextRendererMap.end();
                    it++
            ) {
                renderText(camera, it->second);
            }

            popOpenGLState();
        }

        popOpenGLState();
    }
}

void RenderingEngine::renderMeshWithAllRequiredShaders(
        const std::shared_ptr<CameraComponent>& camera,
        const std::shared_ptr<OpenGlMeshRendererComponent>& meshRenderer,
        const std::unordered_map<std::string, std::shared_ptr<AmbientLightComponent>>& layerNameToAmbientLightMap,
        const std::string& layerName
) {
    auto shaderProgramContainer = m_shadersRepository->getShaderProgramContainer("unlit");
    glUseProgram(shaderProgramContainer.shaderProgram());
    renderMesh(camera, meshRenderer, ShaderType::UNLIT, shaderProgramContainer);

    const auto& ambientLight = layerNameToAmbientLightMap.at(layerName);
    throwErrorIfNull(ambientLight, "No ambient light found");
    shaderProgramContainer = m_shadersRepository->getShaderProgramContainer("ambient");
    glUseProgram(shaderProgramContainer.shaderProgram());
    if (auto ambientColorUniform = shaderProgramContainer.ambientColorUniform(); ambientColorUniform >= 0) {
        auto ambientColor = ambientLight->color();
        glUniform3f(
                ambientColorUniform,
                ambientColor.r,
                ambientColor.g,
                ambientColor.b
        );
    }
    renderMesh(camera, meshRenderer, ShaderType::LIGHT, shaderProgramContainer);
}

void RenderingEngine::renderMesh(
        const std::shared_ptr<CameraComponent>& camera,
        const std::shared_ptr<OpenGlMeshRendererComponent>& meshRenderer,
        ShaderType shaderType,
        const OpenGlShaderProgramContainer& shaderProgramContainer
) {
    auto gameObject = meshRenderer->gameObject();
    if (gameObject == nullptr) {
        throw std::domain_error("Mesh renderer is not attached to any game object");
    }

    auto transform = std::static_pointer_cast<TransformationComponent>(
            gameObject->findComponent(TransformationComponent::TYPE_NAME)
    );
    if (transform == nullptr) {
        std::stringstream ss;
        ss << "Being rendered game object " << gameObject->name() << " has no transform component";
        throw std::domain_error(ss.str());
    }


    auto modelMatrix = glm::translate(glm::identity<glm::mat4>(), transform->position());
    modelMatrix *= glm::toMat4(transform->rotation());
    modelMatrix = glm::scale(modelMatrix, transform->scale());

    meshRenderer->render(
            shaderProgramContainer,
            modelMatrix,
            camera->calculateViewMatrix(),
            camera->calculateProjectionMatrix(),
            shaderType
    );
}

void RenderingEngine::renderText(
        const std::shared_ptr<CameraComponent>& camera,
        const std::shared_ptr<OpenGLFreeTypeTextRendererComponent>& textRenderer
) {
    auto gameObject = textRenderer->gameObject();
    if (gameObject == nullptr) {
        throw std::domain_error("Text renderer is not attached to any game object");
    }

    auto transform = std::static_pointer_cast<TransformationComponent>(
            gameObject->findComponent(TransformationComponent::TYPE_NAME)
    );
    if (transform == nullptr) {
        std::stringstream ss;
        ss << "Text's game object " << gameObject->name() << " has no transform component";
        throw std::domain_error(ss.str());
    }

    auto shaderProgramContainer = m_shadersRepository->getShaderProgramContainer("text");
    glUseProgram(shaderProgramContainer.shaderProgram());

    auto modelMatrix = glm::translate(glm::identity<glm::mat4>(), transform->position());
    modelMatrix *= glm::toMat4(transform->rotation());
    modelMatrix = glm::scale(modelMatrix, transform->scale());

    textRenderer->render(
            shaderProgramContainer,
            modelMatrix,
            camera->calculateViewMatrix(),
            camera->calculateProjectionMatrix()
    );
}

void RenderingEngine::onOpenGlContextRecreated() {
    m_shadersRepository->restoreShaders();
    m_geometryBuffersStorage->restoreBuffers();
    m_texturesRepository->restoreTextures();
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

void RenderingEngine::putMeshInGeometryBuffersIfNecessary(const std::string& name, const Mesh& mesh) {
    if (!m_geometryBuffersStorage->findVbo(name)) {
        std::vector<float> vertexData(mesh.vertices().size() * Vertex::VERTEX_COMPONENTS);

        for (int i = 0; i < mesh.vertices().size(); i++) {
            vertexData[0 + i * Vertex::VERTEX_COMPONENTS] = mesh.vertices()[i].position().x;
            vertexData[1 + i * Vertex::VERTEX_COMPONENTS] = mesh.vertices()[i].position().y;
            vertexData[2 + i * Vertex::VERTEX_COMPONENTS] = mesh.vertices()[i].position().z;

            vertexData[3 + i * Vertex::VERTEX_COMPONENTS] = mesh.vertices()[i].normal().x;
            vertexData[4 + i * Vertex::VERTEX_COMPONENTS] = mesh.vertices()[i].normal().y;
            vertexData[5 + i * Vertex::VERTEX_COMPONENTS] = mesh.vertices()[i].normal().z;

            vertexData[6 + i * Vertex::VERTEX_COMPONENTS] = mesh.vertices()[i].uv().x;
            vertexData[7 + i * Vertex::VERTEX_COMPONENTS] = mesh.vertices()[i].uv().y;
        }

        m_geometryBuffersStorage->createStaticVertexBuffer(name, vertexData);
    }

    if (!m_geometryBuffersStorage->findIbo(name)) {
        m_geometryBuffersStorage->createStaticIndexBuffer(name, mesh.indices());
    }
}
