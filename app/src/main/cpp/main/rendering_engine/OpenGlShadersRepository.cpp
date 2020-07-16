//
// Created by Igor Lapin on 08/07/2020.
//

#include <sstream>
#include <exception>
#include <GLES2/gl2.h>
#include "OpenGlShadersRepository.h"
#include "ShaderCreationParams.h"

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

OpenGlShaderProgramContainer OpenGlShadersRepository::getShaderProgramContainer(const std::string &name) const {
    if (m_shaderProgramContainers.count(name) == 0) {
        std::stringstream ss;
        ss << "Shader program " << name << " does not exist";
        throw std::domain_error(ss.str());
    }

    return m_shaderProgramContainers.at(name);
}

GLuint OpenGlShadersRepository::createVertexShader(const std::string& name, const std::string& source) {
    return createVertexShader(name, source, false);
}

GLuint OpenGlShadersRepository::createFragmentShader(const std::string &name, const std::string &source) {
    return createFragmentShader(name, source, false);
}

OpenGlShaderProgramContainer OpenGlShadersRepository::createShaderProgram(
        const std::string& name,
        const std::string& vertexShaderName,
        const std::string& fragmentShaderName
) {
    return createShaderProgram(name, vertexShaderName, fragmentShaderName, false);
}

void OpenGlShadersRepository::removeAllShadersAndPrograms() {
    for (auto& entry : m_shaderProgramContainers) {
        glDeleteProgram(entry.second.shaderProgram());
    }
    for (auto& entry : m_vertexShaders) {
        glDeleteShader(entry.second);
    }
    for (auto& entry : m_fragmentShaders) {
        glDeleteShader(entry.second);
    }

    m_shadersCreationParams.clear();
    m_vertexShaders.clear();
    m_fragmentShaders.clear();
}

void OpenGlShadersRepository::restoreShaders() {
    m_vertexShaders.clear();
    m_fragmentShaders.clear();
    m_shaderProgramContainers.clear();

    for (auto& shaderCreationParams : m_shadersCreationParams) {
        if (std::holds_alternative<VertexShaderCreationParams>(shaderCreationParams)) {
            auto vertexShaderCreationParams = std::get<VertexShaderCreationParams>(shaderCreationParams);
            createVertexShader(vertexShaderCreationParams.name, vertexShaderCreationParams.source, true);
        } else if (std::holds_alternative<FragmentShaderCreationParams>(shaderCreationParams)) {
            auto fragmentShaderCreationParams = std::get<FragmentShaderCreationParams>(shaderCreationParams);
            createFragmentShader(fragmentShaderCreationParams.name, fragmentShaderCreationParams.source, true);
        } else if (std::holds_alternative<ShaderProgramCreationParams>(shaderCreationParams)) {
            auto shaderProgramCreationParams = std::get<ShaderProgramCreationParams>(shaderCreationParams);
            createShaderProgram(
                    shaderProgramCreationParams.name,
                    shaderProgramCreationParams.vertexShaderName,
                    shaderProgramCreationParams.fragmentShaderName,
                    true
            );
        }
    }
}

GLuint OpenGlShadersRepository::createVertexShader(
        const std::string &name,
        const std::string &source,
        bool isBeingRestored
) {
    if (m_vertexShaders.count(name) > 0) {
        std::stringstream ss;
        ss << "Vertex shader " << name << " already exists";
        throw std::domain_error(ss.str());
    }

    auto shader = glCreateShader(GL_VERTEX_SHADER);
    if (shader == 0) {
        std::stringstream ss;
        ss << "Failed to allocate vertex shader " << name;
        throw std::domain_error(ss.str());
    }
    const GLchar* strings[1];
    GLint stringSizes[1];
    strings[0] = source.c_str();
    stringSizes[0] = source.size();
    glShaderSource(shader, 1, strings, stringSizes);
    glCompileShader(shader);

    m_vertexShaders[name] = shader;
    if (!isBeingRestored) {
        m_shadersCreationParams.emplace_back(VertexShaderCreationParams { name, source });
    }

    m_openGLErrorDetector->checkShaderCompilationError(shader, "createVertexShader");

    return shader;
}

GLuint OpenGlShadersRepository::createFragmentShader(
        const std::string& name,
        const std::string& source,
        bool isBeingRestored
) {
    if (m_fragmentShaders.count(name) > 0) {
        std::stringstream ss;
        ss << "Fragment shader " << name << " already exists";
        throw std::domain_error(ss.str());
    }

    auto shader = glCreateShader(GL_FRAGMENT_SHADER);
    if (shader == 0) {
        std::stringstream ss;
        ss << "Failed to allocate fragment shader " << name;
        throw std::domain_error(ss.str());
    }
    const GLchar* strings[1];
    GLint stringSizes[1];
    strings[0] = source.c_str();
    stringSizes[0] = source.size();
    glShaderSource(shader, 1, strings, stringSizes);
    glCompileShader(shader);

    m_fragmentShaders[name] = shader;
    if (!isBeingRestored) {
        m_shadersCreationParams.emplace_back(FragmentShaderCreationParams { name, source });
    }

    m_openGLErrorDetector->checkShaderCompilationError(shader, "createFragmentShader");

    return shader;
}

OpenGlShaderProgramContainer OpenGlShadersRepository::createShaderProgram(
        const std::string& name,
        const std::string& vertexShaderName,
        const std::string& fragmentShaderName,
        bool isRestoring
) {
    if (m_shaderProgramContainers.count(name) > 0) {
        std::stringstream ss;
        ss << "Shader program " << name << " already exists";
        throw std::domain_error(ss.str());
    }

    auto shaderProgram = glCreateProgram();
    if (shaderProgram == 0) {
        std::stringstream ss;
        ss << "Failed to allocate shader program " << name;
        throw std::domain_error(ss.str());
    }

    auto vertexShader = getVertexShader(vertexShaderName);
    auto fragmentShader = getFragmentShader(fragmentShaderName);

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    auto shaderProgramContainer = OpenGlShaderProgramContainer(m_openGLErrorDetector, shaderProgram);
    m_shaderProgramContainers[name] = shaderProgramContainer;
    if (!isRestoring) {
        m_shadersCreationParams.emplace_back(
                ShaderProgramCreationParams { name, vertexShaderName, fragmentShaderName }
        );
    }

    m_openGLErrorDetector->checkShaderLinkingError(shaderProgram, "createShaderProgram");
    m_openGLErrorDetector->checkOpenGLErrors("createShaderProgram");

    return shaderProgramContainer;
}
