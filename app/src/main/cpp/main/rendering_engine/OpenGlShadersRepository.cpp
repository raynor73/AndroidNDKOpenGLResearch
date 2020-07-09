//
// Created by Igor Lapin on 08/07/2020.
//

#include <sstream>
#include <exception>
#include "OpenGlShadersRepository.h"

GLuint OpenGlShadersRepository::getVertexShader(const std::string &name) const {
    if (m_vertexShaders.count(name) == 0) {
        std::stringstream ss;
        ss << "Vertex shader " << name << " does not exist";
        throw std::domain_error(ss.str());
    }

    return m_vertexShaders.at(name);
}

GLuint OpenGlShadersRepository::getFragmentShader(const std::string &name) const {
    if (m_fragmentShaders.count(name) == 0) {
        std::stringstream ss;
        ss << "Fragment shader " << name << " does not exist";
        throw std::domain_error(ss.str());
    }

    return m_fragmentShaders.at(name);
}

OpenGlShaderProgramContainer OpenGlShadersRepository::getShaderProgram(const std::string &name) const {
    if (m_shaderProgramContainers.count(name) == 0) {
        std::stringstream ss;
        ss << "Shader program " << name << " does not exist";
        throw std::domain_error(ss.str());
    }

    return m_shaderProgramContainers.at(name);
}

GLuint OpenGlShadersRepository::createVertexShader(const std::string &name, const std::string &source) {
    if (m_vertexShaders.count(name) > 0) {
        std::stringstream ss;
        ss << "Vertex shader " << name << " already exists";
        throw std::domain_error(ss.str());
    }

    shadersCreationParams += ShaderCreationParams.VertexShader(name, source)

    val shader = GLES20.glCreateShader(GLES20.GL_VERTEX_SHADER)
    GLES20.glShaderSource(shader, source)
    GLES20.glCompileShader(shader)

    vertexShaders[name] = shader

    openGLErrorDetector.dispatchShaderCompilationError(shader, "createVertexShader")
}

GLuint OpenGlShadersRepository::createFragmentShader(const std::string &name, const std::string &source) {
    return 0;
}

void OpenGlShadersRepository::createShaderProgram(const std::string &name, GLuint vertexShader, GLuint fragmentShader) {

}

void OpenGlShadersRepository::removeAllShadersAndPrograms() {

}
