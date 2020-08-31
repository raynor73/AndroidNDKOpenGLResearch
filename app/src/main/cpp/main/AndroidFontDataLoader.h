//
// Created by Igor Lapin on 20/07/2020.
//

#ifndef ANDROID_NDK_OPENGL_RESEARCH_ANDROIDFONTDATALOADER_H
#define ANDROID_NDK_OPENGL_RESEARCH_ANDROIDFONTDATALOADER_H


#include <jni.h>
#include <engine_3d/FontDataLoader.h>

class AndroidFontDataLoader : public FontDataLoader {

    JavaVM* m_javaVm;
    jclass m_binaryDataLoaderClass;
    jobject m_binaryDataLoaderObject;

public:
    AndroidFontDataLoader(
            JavaVM* javaVm,
            jclass binaryDataLoaderClass,
            jobject binaryDataLoaderObject
    ) : m_javaVm(javaVm),
        m_binaryDataLoaderClass(binaryDataLoaderClass),
        m_binaryDataLoaderObject(binaryDataLoaderObject) {}
    AndroidFontDataLoader(const AndroidFontDataLoader& other) = delete;
    AndroidFontDataLoader(AndroidFontDataLoader&& other) = delete;

    virtual std::vector<uint8_t> loadFontData(const std::string& fontPath) override;

    AndroidFontDataLoader& operator=(const AndroidFontDataLoader& other) = delete;
    AndroidFontDataLoader&& operator=(AndroidFontDataLoader&& other) = delete;
};


#endif //ANDROID_NDK_OPENGL_RESEARCH_ANDROIDFONTDATALOADER_H
