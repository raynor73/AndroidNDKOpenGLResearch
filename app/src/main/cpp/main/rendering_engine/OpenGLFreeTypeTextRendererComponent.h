//
// Created by Igor Lapin on 18/07/2020.
//

#ifndef RENDERING_ENGINE_OPENGL_FREETYPE_TEXT_RENDERER_COMPONENT_H
#define RENDERING_ENGINE_OPENGL_FREETYPE_TEXT_RENDERER_COMPONENT_H


#include <main/rendering_engine/OpenGlShaderProgramContainer.h>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <engine_3d/GameObjectComponent.h>
#include <memory>
#include <utility>
#include <engine_3d/Character.h>
#include "OpenGLVerticalQuadBuffersRepository.h"
#include "OpenGLFreeTypeCharactersRepository.h"
#include "OpenGLTexturesRepository.h"
#include "main/OpenGLErrorDetector.h"
#include "IboInfo.h"

class OpenGLFreeTypeTextRendererComponent : public GameObjectComponent {

    std::vector<std::string> m_layerNames;
    std::shared_ptr<OpenGLFreeTypeCharactersRepository> m_charactersRepository;
    std::shared_ptr<OpenGLVerticalQuadBuffersRepository> m_verticalQuadBuffersRepository;
    std::shared_ptr<OpenGLTexturesRepository> m_texturesRepository;
    std::shared_ptr<OpenGLErrorDetector> m_openGLErrorDetector;

public:
    static const std::string TYPE_NAME;

    OpenGLFreeTypeTextRendererComponent(
            std::vector<std::string> layerNames,
            std::shared_ptr<OpenGLFreeTypeCharactersRepository> charactersRepository,
            std::shared_ptr<OpenGLVerticalQuadBuffersRepository> verticalQuadBuffersRepository,
            std::shared_ptr<OpenGLTexturesRepository> texturesRepository,
            std::shared_ptr<OpenGLErrorDetector> openGLErrorDetector
    ) : m_layerNames(std::move(layerNames)),
        m_charactersRepository(charactersRepository),
        m_verticalQuadBuffersRepository(verticalQuadBuffersRepository),
        m_texturesRepository(texturesRepository),
        m_openGLErrorDetector(openGLErrorDetector) {}

    virtual const std::string& typeName() const override { return TYPE_NAME; }

    const std::vector<std::string>& layerNames() const { return m_layerNames; }
    void setLayerNames(std::vector<std::string> layerNames) { m_layerNames = std::move(layerNames); }

    void render(
            const OpenGlShaderProgramContainer& shaderProgramContainer,
            const glm::mat4x4& modelMatrix,
            const glm::mat4x4& viewMatrix,
            const glm::mat4x4& projectionMatrix
    );

    virtual std::shared_ptr<GameObjectComponent> clone() override;

private:
    void renderCharacter(
            const Character& character,
            float textSize,
            const glm::vec4& color,
            const OpenGlShaderProgramContainer& shaderProgramContainer,
            const glm::mat4x4& modelMatrix,
            const glm::mat4x4& viewMatrix,
            const glm::mat4x4& projectionMatrix,
            float positionX
    );
};


#endif //RENDERING_ENGINE_OPENGL_FREETYPE_TEXT_RENDERER_COMPONENT_H
