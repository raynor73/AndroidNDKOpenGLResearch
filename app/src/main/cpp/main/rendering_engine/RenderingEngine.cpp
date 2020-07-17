//
// Created by Igor Lapin on 28/06/2020.
//

#include <vector>
#include <glm/gtx/compatibility.hpp>
#include <glm/mat4x4.hpp>
#include <engine_3d/CameraComponent.h>
#include <engine_3d/OrthoCameraComponent.h>
#include <engine_3d/MeshComponent.h>
#include <main/rendering_engine/OpenGlMeshRendererComponent.h>
#include <unordered_map>
#include <main/L.h>
#include <main/Constants.h>
#include "RenderingEngine.h"

static GLuint vbo;
static GLuint ibo;
static int numberOfIndices;

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



    float vertexData[] = {
            -0.5, -0.5, 0,
            -0.5,  0.5, 0,
             0.5,  0.5, 0,
             0.5, -0.5, 0
    };
    uint16_t indices[] = {
            0, 2, 1, 0, 3, 2
    };
    numberOfIndices = sizeof(indices) / sizeof(uint16_t);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(
            GL_ARRAY_BUFFER,
            sizeof(vertexData),
            vertexData,
            GL_STATIC_DRAW
    );
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            sizeof(indices),
            indices,
            GL_STATIC_DRAW
    );
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);




    m_openGLErrorDetector->checkOpenGLErrors("RenderingEngine::RenderingEngine");
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

    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_SCISSOR_TEST);

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    auto shaderContainer = m_shadersRepository->getShaderProgramContainer("unlit");
    glUseProgram(shaderContainer.shaderProgram());

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    auto vertexPositionAttribute = shaderContainer.positionAttribute();
    glVertexAttribPointer(
            vertexPositionAttribute,
            Vertex::VERTEX_POSITION_COMPONENTS,
            GL_FLOAT,
            false,
            Vertex::VERTEX_POSITION_COMPONENTS * sizeof(float),
            reinterpret_cast<void*>(0)
    );
    glEnableVertexAttribArray(vertexPositionAttribute);

    auto projectionMatrix = glm::perspective(90.0f, 1080.0f / 1920.0f, 0.1f, 1000.0f);
    auto viewMatrix = glm::lookAt(glm::vec3(0, 0, 1), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
    glm::mat4x4 mvpMatrix = projectionMatrix * viewMatrix;
    glUniformMatrix4fv(shaderContainer.mvpMatrixUniform(), 1, false, &mvpMatrix[0][0]);
    glUniform4f(
            shaderContainer.diffuseColorUniform(),
            1,
            1,
            1,
            1
    );
    glDrawElements(
            GL_TRIANGLES,
            numberOfIndices,
            GL_UNSIGNED_SHORT,
            reinterpret_cast<void*>(0)
    );

    glDisableVertexAttribArray(vertexPositionAttribute);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    m_openGLErrorDetector->checkOpenGLErrors("RenderingEngine::render");

    /*std::vector<std::shared_ptr<CameraComponent>> activeCameras;
    std::unordered_multimap<std::string, std::shared_ptr<OpenGlMeshRendererComponent>> layerNameToMeshRendererMap;

    traverseSceneHierarchy(*scene.rootGameObject(), [&](GameObject& gameObject) {
        if (auto camera = gameObject.findComponent(OrthoCameraComponent::TYPE_NAME); camera != nullptr) {
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
                layerNameToMeshRendererMap.insert({layerName, meshRenderer });
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
                auto shaderProgramContainer = m_shadersRepository->getShaderProgramContainer("unlit");
                glUseProgram(shaderProgramContainer.shaderProgram());
                it->second->render(
                        shaderProgramContainer,
                        glm::identity<glm::mat4x4>(),
                        camera->calculateViewMatrix(),
                        camera->calculateProjectionMatrix()
                );
            }
        }

        popOpenGLState();
    }*/
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
