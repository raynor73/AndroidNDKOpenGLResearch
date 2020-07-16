//
// Created by Igor Lapin on 04/06/2020.
//

#include <GLES/gl.h>
#include <sstream>
#include "OpenGLErrorDetector.h"
#include "L.h"

OpenGLErrorDetector::OpenGLErrorDetector() : m_isOpenGLErrorDetected(false) {
    m_infoBuffer = new GLchar[INFO_BUFFER_SIZE];
}

OpenGLErrorDetector::~OpenGLErrorDetector() {
    delete m_infoBuffer;
}

void OpenGLErrorDetector::checkOpenGLErrors(const std::string &locationName) {
    auto error = glGetError();
    while (error != GL_NO_ERROR) {
        m_isOpenGLErrorDetected = true;

        std::string errorDescription;
        if (s_openGLErrorMap.count(error) > 0) {
            errorDescription = s_openGLErrorMap.at(error);
        } else {
            std::stringstream ss;
            ss << "Unknown error " << error;
            errorDescription = ss.str();
        }
        std::stringstream ss;
        ss << "OpenGL error detected at " << locationName << ": " << errorDescription;
        L::d(LOG_TAG, ss.str());

        error = glGetError();
    }
}

void OpenGLErrorDetector::checkShaderCompilationError(GLuint shader, const std::string &locationName) {
    GLint compileStatus;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
    if (compileStatus == GL_FALSE) {
        m_isOpenGLErrorDetected = true;

        glGetShaderInfoLog(shader, INFO_BUFFER_SIZE, nullptr, m_infoBuffer);
        std::stringstream ss;
        ss << "OpenGL shader compilation failure detected at " << locationName << ": " << m_infoBuffer;
        L::d(LOG_TAG, ss.str());
    }
}

void OpenGLErrorDetector::checkShaderLinkingError(GLuint shader, const std::string &locationName) {
    GLint linkStatus;
    glGetProgramiv(shader, GL_LINK_STATUS, &linkStatus);
    if (linkStatus == GL_FALSE) {
        m_isOpenGLErrorDetected = true;

        std::stringstream ss;
        ss << "OpenGL shader linking failure detected at " << locationName;
        L::d(LOG_TAG, ss.str());
    }
}

void OpenGLErrorDetector::checkFramebufferStatus(const std::string &locationName) {
    auto framebufferStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (framebufferStatus != GL_FRAMEBUFFER_COMPLETE) {
        m_isOpenGLErrorDetected = true;
        std::string statusDescription;
        if (s_framebufferStatusMap.count(framebufferStatus) > 0) {
            statusDescription = s_framebufferStatusMap.at(framebufferStatus);
        } else {
            std::stringstream ss;
            ss << "Unknown status " << framebufferStatus;
            statusDescription = ss.str();
        }
        std::stringstream ss;
        ss << "Incomplete framebuffer status at " << locationName << ": " << statusDescription;
        L::d(LOG_TAG, ss.str());
    }
}

const std::unordered_map<GLenum, const std::string> OpenGLErrorDetector::s_openGLErrorMap {
        { GL_INVALID_ENUM, "GL_INVALID_ENUM" },
        { GL_INVALID_VALUE, "GL_INVALID_VALUE" },
        { GL_INVALID_OPERATION, "GL_INVALID_OPERATION" },
        { GL_STACK_OVERFLOW, "GL_STACK_OVERFLOW" },
        { GL_STACK_UNDERFLOW, "GL_STACK_UNDERFLOW" },
        { GL_OUT_OF_MEMORY, "GL_OUT_OF_MEMORY" },
        { GL_INVALID_FRAMEBUFFER_OPERATION, "GL_INVALID_FRAMEBUFFER_OPERATION" }
};

const std::unordered_map<GLenum, const std::string> OpenGLErrorDetector::s_framebufferStatusMap {
        { GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT, "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT" },
        { GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT, "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT" },
        { GL_FRAMEBUFFER_UNSUPPORTED, "GL_FRAMEBUFFER_UNSUPPORTED" }
};

const std::string OpenGLErrorDetector::LOG_TAG = "OpenGLErrorDetector";
