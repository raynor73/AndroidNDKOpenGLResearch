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
#include "main/OpenGLErrorDetector.h"
#include "IboInfo.h"

class OpenGLFreeTypeTextRendererComponent : public GameObjectComponent {

    std::shared_ptr<OpenGLVerticalQuadBuffersRepository> m_verticalQuadBuffersRepository;
    std::shared_ptr<OpenGLFreeTypeCharactersRepository> m_charactersRepository;
    std::shared_ptr<OpenGLErrorDetector> m_openGLErrorDetector;

public:
    OpenGLFreeTypeTextRendererComponent(
            std::shared_ptr<OpenGLVerticalQuadBuffersRepository> verticalQuadBuffersRepository,
            std::shared_ptr<OpenGLFreeTypeCharactersRepository> charactersRepository,
            std::shared_ptr<OpenGLErrorDetector> openGLErrorDetector
    ) : m_verticalQuadBuffersRepository(verticalQuadBuffersRepository),
        m_charactersRepository(charactersRepository),
        m_openGLErrorDetector(openGLErrorDetector) {}

    void render(
            const OpenGlShaderProgramContainer& shaderProgramContainer,
            const glm::mat4x4& modelMatrix,
            const glm::mat4x4& viewMatrix,
            const glm::mat4x4& projectionMatrix
    );

private:
    void renderCharacter(
            const Character& character,
            const glm::vec4& color,
            const OpenGlShaderProgramContainer& shaderProgramContainer,
            const glm::mat4x4& modelMatrix,
            const glm::mat4x4& viewMatrix,
            const glm::mat4x4& projectionMatrix,
            float positionX
    );
};


#endif //RENDERING_ENGINE_OPENGL_FREETYPE_TEXT_RENDERER_COMPONENT_H
