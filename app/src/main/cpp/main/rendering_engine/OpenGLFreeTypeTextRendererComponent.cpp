//
// Created by Igor Lapin on 18/07/2020.
//

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


}
