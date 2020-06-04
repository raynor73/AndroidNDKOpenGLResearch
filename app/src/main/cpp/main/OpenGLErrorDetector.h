//
// Created by Igor Lapin on 04/06/2020.
//

#ifndef ANDROID_NDK_OPENGL_RESEARCH_OPENGLERRORDETECTOR_H
#define ANDROID_NDK_OPENGL_RESEARCH_OPENGLERRORDETECTOR_H


#include <unordered_map>
#include <string>
#include <GLES2/gl2.h>

class OpenGLErrorDetector {

public:
    OpenGLErrorDetector();

    bool isOpenGLErrorDetected() const { return m_isOpenGLErrorDetected; }

    void checkOpenGLErrors(const std::string &locationName);
    /*void checkShaderCompilationError(int shader, const std::string &locationName);
    void checkShaderLinkingError(int shader, const std::string &locationName);
    void checkFramebufferStatus(const std::string &locationName);*/

private:
    bool m_isOpenGLErrorDetected;

    static const std::unordered_map<GLenum, const std::string> s_openGLErrorMap;
    static const std::unordered_map<GLenum, const std::string> s_framebufferStatusMap;

    static const std::string LOG_TAG;
};


#endif //ANDROID_NDK_OPENGL_RESEARCH_OPENGLERRORDETECTOR_H
