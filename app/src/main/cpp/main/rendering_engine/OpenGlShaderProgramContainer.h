//
// Created by Igor Lapin on 08/07/2020.
//

#ifndef RENDERING_ENGINE_SHADER_PROGRAM_CONTAINER_H
#define RENDERING_ENGINE_SHADER_PROGRAM_CONTAINER_H


#include <GLES2/gl2.h>
#include <main/OpenGLErrorDetector.h>

class OpenGlShaderProgramContainer {

    std::shared_ptr<OpenGLErrorDetector> m_openGlErrorDetector;

    GLuint m_shaderProgram;

    GLint m_positionAttribute;
    GLint m_normalAttribute;
    GLint m_uvAttribute;

    GLint m_mvpMatrixUniform;
    GLint m_textureUniform;
    GLint m_diffuseColorUniform;
    GLint m_useDiffuseColorUniform;

    GLint m_hasSkeletalAnimationUniform;

public:
    OpenGlShaderProgramContainer() = default;
    OpenGlShaderProgramContainer(std::shared_ptr<OpenGLErrorDetector> openGlErrorDetector, GLuint shaderProgram);

    GLuint shaderProgram() const { return m_shaderProgram; }

    GLint positionAttribute() const { return m_positionAttribute; }
    GLint normalAttribute() const { return m_normalAttribute; }
    GLint uvAttribute() const { return m_uvAttribute; }

    GLint mvpMatrixUniform() const { return m_mvpMatrixUniform; }
    GLint textureUniform() const { return m_textureUniform; }
    GLint diffuseColorUniform() const { return m_diffuseColorUniform; }

    GLint useDiffuseColorUniform() const { return m_useDiffuseColorUniform; }

    GLint hasSkeletalAnimationUniform() const { return m_hasSkeletalAnimationUniform; }
};


#endif //RENDERING_ENGINE_SHADER_PROGRAM_CONTAINER_H
