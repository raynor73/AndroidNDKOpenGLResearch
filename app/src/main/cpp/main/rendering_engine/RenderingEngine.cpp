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
#include <engine_3d/CollisionsInfoComponent.h>
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

    auto directionalLightVertexShaderSource = shaderSourcePreprocessor->loadShaderSource(
            "shaders/light/directionalLightVertexShader.glsl"
    );
    auto directionalLightFragmentShaderSource = shaderSourcePreprocessor->loadShaderSource(
            "shaders/light/directionalLightFragmentShader.glsl"
    );
    shadersRepository->createVertexShader("directionalLight", directionalLightVertexShaderSource);
    shadersRepository->createFragmentShader("directionalLight", directionalLightFragmentShaderSource);
    shadersRepository->createShaderProgram("directionalLight", "directionalLight", "directionalLight");

    setupOpenGL();
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
    std::unordered_multimap<std::string, std::shared_ptr<DirectionalLightComponent>> layerNameToDirectionalLightsMap;
    std::unordered_multimap<std::string, std::shared_ptr<OpenGlMeshRendererComponent>> layerNameToMeshRenderersMap;
    std::unordered_multimap<std::string, std::shared_ptr<OpenGlMeshRendererComponent>> layerNameToTranslucentMeshRenderersMap;
    std::unordered_multimap<std::string, std::shared_ptr<OpenGLFreeTypeTextRendererComponent>> layerNameToTextRenderersMap;

    traverseSceneHierarchy(*scene.rootGameObject(), [&](GameObject& gameObject) {
        // TODO This is very very bad to update physics related components in Rendering Engine. This added here to not to traverse whole hierarchy multiple times. But this should be moved out from here ASAP.
        if (auto collisionsInfo = gameObject.findComponent<CollisionsInfoComponent>(); collisionsInfo != nullptr) {
            collisionsInfo->collisions.clear();
        }

        if (
                auto camera = gameObject.findComponent(OrthoCameraComponent::TYPE_NAME);
                camera != nullptr && camera->isEnabled()
        ) {
            activeCameras.push_back(std::static_pointer_cast<CameraComponent>(camera));
        }

        if (
                auto camera = gameObject.findComponent(PerspectiveCameraComponent::TYPE_NAME);
                camera != nullptr && camera->isEnabled()
        ) {
            activeCameras.push_back(std::static_pointer_cast<CameraComponent>(camera));
        }

        if (
                auto meshRenderer = gameObject.findComponent<OpenGlMeshRendererComponent>();
                meshRenderer != nullptr && meshRenderer->isEnabled()
        ) {
            for (auto& layerName : meshRenderer->layerNames()) {
                auto materialComponent = std::static_pointer_cast<MaterialComponent>(
                        gameObject.findComponent(MaterialComponent::TYPE_NAME)
                );
                throwErrorIfNull(materialComponent, "Can't find material component for mesh renderer while rendering");
                if (materialComponent->material().isTranslucent) {
                    layerNameToTranslucentMeshRenderersMap.insert({layerName, meshRenderer });
                } else {
                    layerNameToMeshRenderersMap.insert({layerName, meshRenderer });
                }
            }
        }

        if (
                auto textRenderer = gameObject.findComponent<OpenGLFreeTypeTextRendererComponent>();
                textRenderer != nullptr && textRenderer->isEnabled()
        ) {
            for (auto& layerName : textRenderer->layerNames()) {
                layerNameToTextRenderersMap.insert({layerName, textRenderer });
            }
        }

        if (
                auto meshComponent = gameObject.findComponent<MeshComponent>();
                meshComponent != nullptr && meshComponent->isEnabled()
        ) {
            putMeshInGeometryBuffersIfNecessary(meshComponent->meshName(), meshComponent->mesh());
        }

        if (
                auto ambientLight = gameObject.findComponent<AmbientLightComponent>();
                ambientLight != nullptr && ambientLight->isEnabled()
        ) {
            for (auto& layerName : ambientLight->layerNames()) {
                layerNameToAmbientLightMap.insert({ layerName, ambientLight });
            }
        }

        if (
                auto directionalLight = gameObject.findComponent<DirectionalLightComponent>();
                directionalLight != nullptr && directionalLight->isEnabled()
        ) {
            for (auto& layerName : directionalLight->layerNames()) {
                layerNameToDirectionalLightsMap.insert({layerName, directionalLight });
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
        Viewport viewport { viewportX, viewportY, viewportWidth, viewportHeight };
        Scissor scissor { viewportX, viewportY, viewportWidth, viewportHeight };
        pushOpenGLState({
                viewport,
                scissor,
                true,
                {GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA},
                true,
                GL_LESS
        });

        auto clearColor = camera->clearColor();
        glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
        glClear(clearMask);

        for (auto& layerName : camera->layerNames()) {
            auto layerRenderersRange = layerNameToMeshRenderersMap.equal_range(layerName);
            for (auto it = layerRenderersRange.first; it != layerRenderersRange.second; it++) {
                renderMeshWithAllRequiredShaders(
                        camera,
                        viewport,
                        scissor,
                        it->second,
                        layerNameToAmbientLightMap,
                        layerNameToDirectionalLightsMap,
                        layerName
                );
            }

            auto layerTranslucentRenderersRange = layerNameToTranslucentMeshRenderersMap.equal_range(layerName);
            for (auto it = layerTranslucentRenderersRange.first; it != layerTranslucentRenderersRange.second; it++) {
                renderMeshWithAllRequiredShaders(
                        camera,
                        viewport,
                        scissor,
                        it->second,
                        layerNameToAmbientLightMap,
                        layerNameToDirectionalLightsMap,
                        layerName
                );
            }

            auto layerTextRenderersRange = layerNameToTextRenderersMap.equal_range(layerName);
            for (auto it = layerTextRenderersRange.first; it != layerTextRenderersRange.second; it++) {
                renderText(camera, it->second);
            }
        }

        popOpenGLState();
    }
}

void RenderingEngine::renderMeshWithAllRequiredShaders(
        const std::shared_ptr<CameraComponent>& camera,
        const Viewport& viewport,
        const Scissor& scissor,
        const std::shared_ptr<OpenGlMeshRendererComponent>& meshRenderer,
        const std::unordered_map<std::string, std::shared_ptr<AmbientLightComponent>>& layerNameToAmbientLightMap,
        const std::unordered_multimap<std::string, std::shared_ptr<DirectionalLightComponent>>& layerNameToDirectionalLightsMap,
        const std::string& layerName
) {
    /*auto material = meshRenderer->gameObject()->findComponent<MaterialComponent>()->material();
    if (material.isUnlit) {
        if (material.isGradient) {
        } else {
            if (material.useDiffuseColor) {

            } else {

            }
        }
    } else {
        if (material.isGradient) {

        } else {
            if (material.useDiffuseColor) {

            } else {

            }
        }
    }*/

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

    pushOpenGLState({
        viewport,
        scissor,
        true,
        {GL_ONE, GL_ONE},
        false,
        GL_EQUAL
    });

    shaderProgramContainer = m_shadersRepository->getShaderProgramContainer("directionalLight");
    glUseProgram(shaderProgramContainer.shaderProgram());

    auto layerDirectionalLightsRange = layerNameToDirectionalLightsMap.equal_range(layerName);
    for (auto it = layerDirectionalLightsRange.first; it != layerDirectionalLightsRange.second; it++) {
        if (auto colorUniform = shaderProgramContainer.directionalLightColorUniform(); colorUniform >= 0) {
            auto color = it->second->color();
            glUniform3f(
                    colorUniform,
                    color.r,
                    color.g,
                    color.b
            );
        }
        if (auto directionUniform = shaderProgramContainer.directionalLightDirectionUniform(); directionUniform >= 0) {
            auto transform = it->second->gameObject()->findComponent<TransformationComponent>();
            throwErrorIfNull(transform, "Directional light have no transform while rendering");
            auto direction = transform->rotation() * it->second->direction();
            glUniform3f(
                    directionUniform,
                    direction.x,
                    direction.y,
                    direction.z
            );
        }
        renderMesh(camera, meshRenderer, ShaderType::LIGHT, shaderProgramContainer);
    }

    popOpenGLState();
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
    setupOpenGL();

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
            vertexData[     i * Vertex::VERTEX_COMPONENTS] = mesh.vertices()[i].position().x;
            vertexData[ 1 + i * Vertex::VERTEX_COMPONENTS] = mesh.vertices()[i].position().y;
            vertexData[ 2 + i * Vertex::VERTEX_COMPONENTS] = mesh.vertices()[i].position().z;

            vertexData[ 3 + i * Vertex::VERTEX_COMPONENTS] = mesh.vertices()[i].normal().x;
            vertexData[ 4 + i * Vertex::VERTEX_COMPONENTS] = mesh.vertices()[i].normal().y;
            vertexData[ 5 + i * Vertex::VERTEX_COMPONENTS] = mesh.vertices()[i].normal().z;

            vertexData[ 6 + i * Vertex::VERTEX_COMPONENTS] = mesh.vertices()[i].uv().x;
            vertexData[ 7 + i * Vertex::VERTEX_COMPONENTS] = mesh.vertices()[i].uv().y;

            vertexData[ 8 + i * Vertex::VERTEX_COMPONENTS] = mesh.vertices()[i].jointIndices().x;
            vertexData[ 9 + i * Vertex::VERTEX_COMPONENTS] = mesh.vertices()[i].jointIndices().y;
            vertexData[10 + i * Vertex::VERTEX_COMPONENTS] = mesh.vertices()[i].jointIndices().z;

            vertexData[11 + i * Vertex::VERTEX_COMPONENTS] = mesh.vertices()[i].jointWeights().x;
            vertexData[12 + i * Vertex::VERTEX_COMPONENTS] = mesh.vertices()[i].jointWeights().y;
            vertexData[13 + i * Vertex::VERTEX_COMPONENTS] = mesh.vertices()[i].jointWeights().z;
        }

        m_geometryBuffersStorage->createStaticVertexBuffer(name, vertexData);
    }

    if (!m_geometryBuffersStorage->findIbo(name)) {
        m_geometryBuffersStorage->createStaticIndexBuffer(name, mesh.indices());
    }
}

void RenderingEngine::setupOpenGL() {
    glEnable(GL_DEPTH_TEST);
    glFrontFace(GL_CCW);
    glEnable(GL_SCISSOR_TEST);
    glClearDepthf(1);
}
