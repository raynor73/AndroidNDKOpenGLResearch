//
// Created by Igor Lapin on 18/07/2020.
//

#ifndef RENDERING_ENGINE_OPENGL_FREETYPE_TEXT_RENDERER_COMPONENT_H
#define RENDERING_ENGINE_OPENGL_FREETYPE_TEXT_RENDERER_COMPONENT_H


#include <main/rendering_engine/OpenGlShaderProgramContainer.h>
#include <glm/mat4x4.hpp>
#include <engine_3d/GameObjectComponent.h>
#include <utility>
#include "IboInfo.h"
#include <engine_3d/Character.h>

class OpenGLFreeTypeTextRendererComponent : public GameObjectComponent {

    GLuint m_rectangleVbo;
    IboInfo m_rectangleIboInfo;

public:
    OpenGLFreeTypeTextRendererComponent(
            GLuint rectangleVbo,
            IboInfo rectangleIboInfo
    ) : m_rectangleVbo(rectangleVbo),
        m_rectangleIboInfo(std::move(rectangleIboInfo)) {}

    void render(
            const OpenGlShaderProgramContainer& shaderProgramContainer,
            const glm::mat4x4& modelMatrix,
            const glm::mat4x4& viewMatrix,
            const glm::mat4x4& projectionMatrix
    );
};


#endif //RENDERING_ENGINE_OPENGL_FREETYPE_TEXT_RENDERER_COMPONENT_H
