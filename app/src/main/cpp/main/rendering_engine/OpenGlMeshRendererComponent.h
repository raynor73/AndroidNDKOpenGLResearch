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

class OpenGlMeshRendererComponent : public GameObjectComponent {

    std::vector<std::string> m_layerNames;
    std::shared_ptr<OpenGLErrorDetector> m_openGlErrorDetector;
    std::shared_ptr<OpenGLGeometryBuffersStorage> m_geometryBuffersStorage;

public:
    static const std::string TYPE_NAME;

    OpenGlMeshRendererComponent(
            std::vector<std::string> layerNames,
            std::shared_ptr<OpenGLGeometryBuffersStorage> geometryBuffersStorage,
            std::shared_ptr<OpenGLErrorDetector> openGlErrorDetector
    ) : m_layerNames(std::move(layerNames)),
        m_geometryBuffersStorage(geometryBuffersStorage),
        m_openGlErrorDetector(openGlErrorDetector)
    {}

    const std::vector<std::string>& layerNames() const { return m_layerNames; }
    void setLayerNames(std::vector<std::string> layerNames) { m_layerNames = std::move(layerNames); }

    void render(
            const OpenGlShaderProgramContainer& shaderProgramContainer,
            const glm::mat4x4& modelMatrix,
            const glm::mat4x4& viewMatrix,
            const glm::mat4x4& projectionMatrix
    );

    virtual const std::string& typeName() const override { return TYPE_NAME; }

    virtual std::shared_ptr<GameObjectComponent> clone() override;
};


#endif //RENDERING_ENGINE_OPENGL_MESH_RENDERER_COMPONENT_H
