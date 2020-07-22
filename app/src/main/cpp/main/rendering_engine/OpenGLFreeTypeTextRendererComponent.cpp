//
// Created by Igor Lapin on 18/07/2020.
//

#include <sstream>
#include <engine_3d/TextComponent.h>
#include <engine_3d/GameObject.h>
#include <glm/gtx/compatibility.hpp>
#include "OpenGLFreeTypeTextRendererComponent.h"

void OpenGLFreeTypeTextRendererComponent::render(
        const OpenGlShaderProgramContainer& shaderProgramContainer,
        const glm::mat4x4& modelMatrix,
        const glm::mat4x4& viewMatrix,
        const glm::mat4x4& projectionMatrix
) {
    if (!m_isEnabled || m_gameObject == nullptr || !m_gameObject->isEnabled()) {
        return;
    }

    auto textComponent = std::static_pointer_cast<TextComponent>(m_gameObject->findComponent(TextComponent::TYPE_NAME));
    if (textComponent == nullptr) {
        std::stringstream ss;
        ss << "No text component found for rendering in game object " << m_gameObject->name();
        throw std::domain_error(ss.str());
    }

    auto text = textComponent->text();
    auto textAppearance = textComponent->textAppearance();
    float positionX = 0;
    for (char c : text) {
        auto character = m_charactersRepository->getCharacter(textAppearance, c);
        renderCharacter(
                character,
                textComponent->textColor(),
                shaderProgramContainer,
                modelMatrix,
                viewMatrix,
                projectionMatrix,
                positionX
        );
        positionX += character.advance() >> 6;
    }
}

void OpenGLFreeTypeTextRendererComponent::renderCharacter(
        const Character& character,
        const glm::vec4& color,
        const OpenGlShaderProgramContainer& shaderProgramContainer,
        const glm::mat4x4& modelMatrix,
        const glm::mat4x4& viewMatrix,
        const glm::mat4x4& projectionMatrix,
        float positionX
) {
    auto buffers = m_verticalQuadBuffersRepository->getBuffers();

    glBindBuffer(GL_ARRAY_BUFFER, buffers.vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers.iboInfo.ibo);

    auto vertexPositionAttribute = shaderProgramContainer.positionAttribute();
    if (vertexPositionAttribute >= 0) {
        glVertexAttribPointer(
                vertexPositionAttribute,
                Vertex::VERTEX_POSITION_COMPONENTS,
                GL_FLOAT,
                false,
                Vertex::VERTEX_COMPONENTS * sizeof(float),
                reinterpret_cast<void*>(0)
        );
        glEnableVertexAttribArray(vertexPositionAttribute);
    }

    auto vertexNormalAttribute = shaderProgramContainer.normalAttribute();
    if (vertexNormalAttribute >= 0) {
        glVertexAttribPointer(
                vertexNormalAttribute,
                Vertex::VERTEX_NORMAL_COMPONENTS,
                GL_FLOAT,
                false,
                Vertex::VERTEX_COMPONENTS * sizeof(float),
                reinterpret_cast<void*>(Vertex::VERTEX_POSITION_COMPONENTS * sizeof(float))
        );
        glEnableVertexAttribArray(vertexNormalAttribute);
    }

    auto vertexUvAttribute = shaderProgramContainer.uvAttribute();
    if (vertexUvAttribute >= 0) {
        glVertexAttribPointer(
                vertexUvAttribute,
                Vertex::VERTEX_UV_COMPONENTS,
                GL_FLOAT,
                false,
                Vertex::VERTEX_COMPONENTS * sizeof(float),
                reinterpret_cast<void*>(
                        (Vertex::VERTEX_POSITION_COMPONENTS + Vertex::VERTEX_NORMAL_COMPONENTS) * sizeof(float)
                )
        );
        glEnableVertexAttribArray(vertexUvAttribute);
    }

    if (auto mvpMatrixUniform = shaderProgramContainer.mvpMatrixUniform(); mvpMatrixUniform >= 0) {
        glm::mat4x4 mvpMatrix = projectionMatrix * viewMatrix * glm::translate(modelMatrix, glm::vec3(positionX, 0, 0));
        glUniformMatrix4fv(mvpMatrixUniform, 1, false, &mvpMatrix[0][0]);
    }

    if (auto diffuseColorUniform = shaderProgramContainer.diffuseColorUniform(); diffuseColorUniform >= 0) {
        glUniform4f(diffuseColorUniform, color.r, color.g, color.b, color.a);
    }

    glDrawElements(
            GL_TRIANGLES,
            buffers.iboInfo.numberOfIndices,
            GL_UNSIGNED_SHORT,
            reinterpret_cast<void*>(0)
    );

    if (vertexPositionAttribute >= 0) {
        glDisableVertexAttribArray(vertexPositionAttribute);
    }
    if (vertexNormalAttribute >= 0) {
        glDisableVertexAttribArray(vertexNormalAttribute);
    }
    if (vertexUvAttribute >= 0) {
        glDisableVertexAttribArray(vertexUvAttribute);
    }

    //glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    m_openGLErrorDetector->checkOpenGLErrors("OpenGLFreeTypeTextRendererComponent::renderCharacter");
}
