//
// Created by Igor Lapin on 08/07/2020.
//

#include <utility>
#include "OpenGlShaderProgramContainer.h"

OpenGlShaderProgramContainer::OpenGlShaderProgramContainer(
        std::shared_ptr<OpenGLErrorDetector> openGlErrorDetector,
        GLuint shaderProgram
) : m_openGlErrorDetector(std::move(openGlErrorDetector)),
    m_shaderProgram(shaderProgram),
    m_positionAttribute(glGetAttribLocation(m_shaderProgram, "positionAttribute")),
    m_normalAttribute(glGetAttribLocation(shaderProgram, "normalAttribute")),
    m_uvAttribute(glGetAttribLocation(shaderProgram, "uvAttribute")),
    m_mvpMatrixUniform(glGetUniformLocation(shaderProgram, "mvpMatrixUniform")),
    m_modelMatrixUniform(glGetUniformLocation(shaderProgram, "modelMatrixUniform")),
    m_textureUniform(glGetUniformLocation(shaderProgram, "textureUniform")),
    m_diffuseColorUniform(glGetUniformLocation(shaderProgram, "diffuseColorUniform")),
    m_useDiffuseColorUniform(glGetUniformLocation(shaderProgram, "useDiffuseColorUniform")),
    m_ambientColorUniform(glGetUniformLocation(shaderProgram, "ambientColorUniform")),
    m_directionalLightColorUniform(glGetUniformLocation(shaderProgram, "directionalLightUniform.color")),
    m_directionalLightDirectionUniform(glGetUniformLocation(shaderProgram, "directionalLightUniform.direction")),
    m_hasSkeletalAnimationUniform(glGetUniformLocation(shaderProgram, "hasSkeletalAnimationUniform"))
{
    m_openGlErrorDetector->checkOpenGLErrors("OpenGlShaderProgramContainer::OpenGlShaderProgramContainer");
}
