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
    GLint m_jointIndicesAttribute;
    GLint m_jointWeightsAttribute;

    GLint m_mvpMatrixUniform;
    GLint m_modelMatrixUniform;
    GLint m_textureUniform;
    GLint m_diffuseColorUniform;
    GLint m_useDiffuseColorUniform;

    GLint m_ambientColorUniform;
    GLint m_directionalLightColorUniform;
    GLint m_directionalLightDirectionUniform;

    GLint m_hasSkeletalAnimationUniform;
    GLint m_jointTransformsUniform;

public:
    OpenGlShaderProgramContainer() = default;
    OpenGlShaderProgramContainer(std::shared_ptr<OpenGLErrorDetector> openGlErrorDetector, GLuint shaderProgram);

    GLuint shaderProgram() const { return m_shaderProgram; }

    GLint positionAttribute() const { return m_positionAttribute; }
    GLint normalAttribute() const { return m_normalAttribute; }
    GLint uvAttribute() const { return m_uvAttribute; }

    GLint jointIndicesAttribute() const { return m_jointIndicesAttribute; }
    GLint jointWeightsAttribute() const { return m_jointWeightsAttribute; }

    GLint mvpMatrixUniform() const { return m_mvpMatrixUniform; }
    GLint modelMatrixUniform() const { return m_modelMatrixUniform; }
    GLint textureUniform() const { return m_textureUniform; }
    GLint diffuseColorUniform() const { return m_diffuseColorUniform; }

    GLint useDiffuseColorUniform() const { return m_useDiffuseColorUniform; }

    GLint ambientColorUniform() const { return m_ambientColorUniform; }
    GLint directionalLightColorUniform() const { return m_directionalLightColorUniform; }
    GLint directionalLightDirectionUniform() const { return m_directionalLightDirectionUniform; }

    GLint hasSkeletalAnimationUniform() const { return m_hasSkeletalAnimationUniform; }
    GLint jointTransformsUniform() const { return m_jointTransformsUniform; }
};


#endif //RENDERING_ENGINE_SHADER_PROGRAM_CONTAINER_H
