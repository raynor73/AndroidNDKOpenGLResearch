//
// Created by Igor Lapin on 04/06/2020.
//

#include <GLES/gl.h>
#include <sstream>
#include "OpenGLErrorDetector.h"
#include "L.h"

OpenGLErrorDetector::OpenGLErrorDetector() : m_isOpenGLErrorDetected(false) {
    // do nothing
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
        ss << "OpenGL error detected at  " << locationName << ": " << errorDescription;
        L::d(LOG_TAG, ss.str());

        error = glGetError();
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