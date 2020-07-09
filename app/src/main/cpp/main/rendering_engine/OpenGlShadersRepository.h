//
// Created by Igor Lapin on 08/07/2020.
//

#ifndef RENDERING_ENGINE_OPENGL_SHADERS_REPOSITORY_H
#define RENDERING_ENGINE_OPENGL_SHADERS_REPOSITORY_H


#include <GLES2/gl2.h>
#include <memory>
#include <unordered_map>
#include <string>
#include <main/OpenGLErrorDetector.h>
#include <variant>
#include <vector>
#include "ShaderCreationParams.h"
#include "OpenGlShaderProgramContainer.h"

class OpenGlShadersRepository {

    std::shared_ptr<OpenGLErrorDetector> m_openGLErrorDetector;

    std::vector<std::variant<VertexShaderCreationParams, FragmentShaderCreationParams, ShaderProgramCreationParams>>
        m_shadersCreationParams;

    std::unordered_map<std::string, GLuint> m_vertexShaders;
    std::unordered_map<std::string, GLuint> m_fragmentShaders;
    std::unordered_map<std::string, OpenGlShaderProgramContainer> m_shaderProgramContainers;

public:
    OpenGlShadersRepository(std::shared_ptr<OpenGLErrorDetector> openGLErrorDetector) :
        m_openGLErrorDetector(openGLErrorDetector) {}
    OpenGlShadersRepository(const OpenGlShadersRepository&) = delete;
    OpenGlShadersRepository(OpenGlShadersRepository&&) = delete;

    GLuint getVertexShader(const std::string& name) const;
    GLuint getFragmentShader(const std::string& name) const;
    OpenGlShaderProgramContainer getShaderProgram(const std::string& name) const;

    GLuint createVertexShader(const std::string& name, const std::string& source);
    GLuint createFragmentShader(const std::string& name, const std::string& source);
    OpenGlShaderProgramContainer createShaderProgram(
            const std::string& name,
            const std::string& vertexShaderName,
            const std::string& fragmentShaderName
    );

    void restoreShaders();

    void removeAllShadersAndPrograms();

    OpenGlShadersRepository& operator=(const OpenGlShadersRepository&) = delete;
    OpenGlShadersRepository& operator=(OpenGlShadersRepository&&) = delete;

private:
    GLuint createVertexShader(const std::string& name, const std::string& source, bool isBeingRestored);
    GLuint createFragmentShader(const std::string& name, const std::string& source, bool isBeingRestored);
    OpenGlShaderProgramContainer createShaderProgram(
            const std::string& name,
            const std::string& vertexShaderName,
            const std::string& fragmentShaderName,
            bool isBeingRestored
    );
};


#endif //RENDERING_ENGINE_OPENGL_SHADERS_REPOSITORY_H
