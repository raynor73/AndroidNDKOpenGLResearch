//
// Created by Igor Lapin on 24/08/2020.
//

#ifndef ANDROID_NDK_OPENGL_RESEARCH_ANDROIDFSABSTRACTION_H
#define ANDROID_NDK_OPENGL_RESEARCH_ANDROIDFSABSTRACTION_H


#include <engine_3d/FsAbstraction.h>
#include <jni.h>

class AndroidFsAbstraction : public FsAbstraction {

    JavaVM* m_javaVm;
    jclass m_bridgeClass;
    jobject m_bridgeObject;

public:
    AndroidFsAbstraction(
            JavaVM* javaVm,
            jclass bridgeClass,
            jobject bridgeObject
    ) : m_javaVm(javaVm),
        m_bridgeClass(bridgeClass),
        m_bridgeObject(bridgeObject)
    {}

    AndroidFsAbstraction(const AndroidFsAbstraction&) = delete;
    AndroidFsAbstraction(AndroidFsAbstraction&&) = delete;

    virtual bool isFileExists(const std::string& path) override;

    virtual std::vector<uint8_t> readBinaryFileContent(const std::string& path) override;

    virtual void writeBinaryFileContent(const std::string& path, const std::vector<uint8_t>& data) override;

    virtual std::string readTextFileContent(const std::string& path) override;

    virtual void writeTextFileContent(const std::string& path, const std::string& text) override;

    virtual void deleteFile(const std::string& path) override;

    AndroidFsAbstraction& operator=(const AndroidFsAbstraction&) = delete;
    AndroidFsAbstraction& operator=(AndroidFsAbstraction&&) = delete;
};


#endif //ANDROID_NDK_OPENGL_RESEARCH_ANDROIDFSABSTRACTION_H
