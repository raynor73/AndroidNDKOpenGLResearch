//
// Created by Igor Lapin on 26/08/2020.
//

#ifndef ANDROID_NDK_OPENGL_RESEARCH_ANDROIDREADONLYFSABSTRACTION_H
#define ANDROID_NDK_OPENGL_RESEARCH_ANDROIDREADONLYFSABSTRACTION_H


#include <engine_3d/ReadOnlyFsAbstraction.h>
#include <jni.h>

class AndroidReadOnlyFsAbstraction : public ReadOnlyFsAbstraction {

    JavaVM* m_javaVm;
    jclass m_bridgeClass;
    jobject m_bridgeObject;

public:
    AndroidReadOnlyFsAbstraction(
            JavaVM* javaVm,
            jclass bridgeClass,
            jobject bridgeObject
    ) : m_javaVm(javaVm),
        m_bridgeClass(bridgeClass),
        m_bridgeObject(bridgeObject)
    {}

    virtual bool isFileExists(const std::string& path) override;

    virtual std::vector<uint8_t> readBinaryFileContent(const std::string& path) override;

    virtual std::string readTextFileContent(const std::string& path) override;
};


#endif //ANDROID_NDK_OPENGL_RESEARCH_ANDROIDREADONLYFSABSTRACTION_H
