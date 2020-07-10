//
// Created by Igor Lapin on 10/07/2020.
//

#ifndef ANDROID_NDK_OPENGL_RESEARCH_ANDROIDSHADERSOURCEREPOSITORY_H
#define ANDROID_NDK_OPENGL_RESEARCH_ANDROIDSHADERSOURCEREPOSITORY_H


#include <jni.h>
#include <main/rendering_engine/ShaderSourceRepository.h>

class AndroidShaderSourceRepository : public ShaderSourceRepository {

    JavaVM* m_javaVm;
    jclass m_textFileLoaderClass;
    jobject m_textFileLoaderObject;

public:
    AndroidShaderSourceRepository(
            JavaVM* javaVm,
            jclass textDataLoaderClass,
            jobject textDataLoaderObject
    ) : m_javaVm(javaVm),
        m_textFileLoaderClass(textDataLoaderClass),
        m_textFileLoaderObject(textDataLoaderObject)
    {}
    AndroidShaderSourceRepository(const AndroidShaderSourceRepository&) = delete;
    AndroidShaderSourceRepository(AndroidShaderSourceRepository&&) = delete;

    virtual std::string loadShaderSource(const std::string& path) override;

    AndroidShaderSourceRepository& operator=(const AndroidShaderSourceRepository&) = delete;
    AndroidShaderSourceRepository& operator=(AndroidShaderSourceRepository&&) = delete;
};


#endif //ANDROID_NDK_OPENGL_RESEARCH_ANDROIDSHADERSOURCEREPOSITORY_H
