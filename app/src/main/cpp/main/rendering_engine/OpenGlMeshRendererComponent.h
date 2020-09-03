//
// Created by Igor Lapin on 03/07/2020.
//

#ifndef RENDERING_ENGINE_OPENGL_MESH_RENDERER_COMPONENT_H
#define RENDERING_ENGINE_OPENGL_MESH_RENDERER_COMPONENT_H


#include <string>
#include <vector>
#include <utility>
#include <memory>
#include <engine_3d/GameObjectComponent.h>
#include <main/OpenGLErrorDetector.h>
#include <glm/mat4x4.hpp>
#include "OpenGLGeometryBuffersStorage.h"
#include "OpenGlShaderProgramContainer.h"
#include "OpenGLTexturesRepository.h"

enum class ShaderType {
    UNLIT, LIGHT, GRADIENT
};

class OpenGlMeshRendererComponent : public GameObjectComponent {

    std::vector<std::string> m_layerNames;
    std::shared_ptr<OpenGLErrorDetector> m_openGlErrorDetector;
    std::shared_ptr<OpenGLGeometryBuffersStorage> m_geometryBuffersStorage;
    std::shared_ptr<OpenGLTexturesRepository> m_texturesRepository;

    glm::vec3 m_topPoint;
    glm::vec3 m_bottomPoint;
    bool m_isTopAndBottomPointsFound = false;

public:
    static const std::string TYPE_NAME;

    OpenGlMeshRendererComponent(
            std::vector<std::string> layerNames,
            std::shared_ptr<OpenGLGeometryBuffersStorage> geometryBuffersStorage,
            std::shared_ptr<OpenGLTexturesRepository> texturesRepository,
            std::shared_ptr<OpenGLErrorDetector> openGlErrorDetector
    ) : m_layerNames(std::move(layerNames)),
        m_geometryBuffersStorage(std::move(geometryBuffersStorage)),
        m_texturesRepository(std::move(texturesRepository)),
        m_openGlErrorDetector(std::move(openGlErrorDetector))
    {}

    const std::vector<std::string>& layerNames() const { return m_layerNames; }
    void setLayerNames(std::vector<std::string> layerNames) { m_layerNames = std::move(layerNames); }

    void render(
            const OpenGlShaderProgramContainer& shaderProgramContainer,
            const glm::mat4x4& modelMatrix,
            const glm::mat4x4& viewMatrix,
            const glm::mat4x4& projectionMatrix,
            ShaderType shaderType
    );

    virtual const std::string& typeName() const override { return TYPE_NAME; }

    virtual std::shared_ptr<GameObjectComponent> clone() override;

private:
    void findTopAndBottomPoints(const Mesh& mesh) {
        m_topPoint = { 0, 0, 0 };
        m_bottomPoint = { 0, 0, 0 };

        for (auto& vertex : mesh.vertices()) {
            auto position = vertex.position();

            if (position.y > m_topPoint.y) {
                m_topPoint.y = position.y;
            }

            if (position.y < m_bottomPoint.y) {
                m_bottomPoint.y = position.y;
            }
        }

        m_isTopAndBottomPointsFound = true;
    }
};


#endif //RENDERING_ENGINE_OPENGL_MESH_RENDERER_COMPONENT_H
