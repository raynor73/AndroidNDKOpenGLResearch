//
// Created by Igor Lapin on 04/06/2020.
//

#ifndef ANDROID_NDK_OPENGL_RESEARCH_OPENGLERRORDETECTOR_H
#define ANDROID_NDK_OPENGL_RESEARCH_OPENGLERRORDETECTOR_H


#include <unordered_map>
#include <string>
#include <GLES2/gl2.h>

class OpenGLErrorDetector {

    bool m_isOpenGLErrorDetected;
    GLchar *m_infoBuffer;

    static const std::unordered_map<GLenum, const std::string> s_openGLErrorMap;
    static const std::unordered_map<GLenum, const std::string> s_framebufferStatusMap;

    static const std::string LOG_TAG;
    static const int INFO_BUFFER_SIZE = 1024;

public:
    OpenGLErrorDetector();
    OpenGLErrorDetector(const OpenGLErrorDetector&) = delete;
    OpenGLErrorDetector(OpenGLErrorDetector&&) = delete;
    virtual ~OpenGLErrorDetector();

    bool isOpenGLErrorDetected() const { return m_isOpenGLErrorDetected; }

    void checkOpenGLErrors(const std::string &locationName);
    void checkShaderCompilationError(GLuint shader, const std::string &locationName);
    void checkShaderLinkingError(GLuint shader, const std::string &locationName);
    void checkFramebufferStatus(const std::string &locationName);

    OpenGLErrorDetector& operator=(const OpenGLErrorDetector&) = delete;
    OpenGLErrorDetector& operator=(OpenGLErrorDetector&&) = delete;
};


#endif //ANDROID_NDK_OPENGL_RESEARCH_OPENGLERRORDETECTOR_H
