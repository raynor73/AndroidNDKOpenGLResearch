//
// Created by Igor Lapin on 02/08/2020.
//

#ifndef ANDROID_NDK_OPENGL_RESEARCH_ANDROIDBITMAPDATALOADER_H
#define ANDROID_NDK_OPENGL_RESEARCH_ANDROIDBITMAPDATALOADER_H


#include <jni.h>
#include <game/BitmapDataLoader.h>

class AndroidBitmapDataLoader : public BitmapDataLoader {

    JavaVM* m_javaVm;
    jclass m_binaryDataLoaderClass;
    jobject m_binaryDataLoaderObject;

public:
    AndroidBitmapDataLoader(
            JavaVM* javaVm,
            jclass binaryDataLoaderClass,
            jobject binaryDataLoaderObject
    ) : m_javaVm(javaVm),
        m_binaryDataLoaderClass(binaryDataLoaderClass),
        m_binaryDataLoaderObject(binaryDataLoaderObject) {}
    AndroidBitmapDataLoader(const AndroidBitmapDataLoader& other) = delete;
    AndroidBitmapDataLoader(AndroidBitmapDataLoader&& other) = delete;

    virtual BitmapInfo loadBitmap(const std::string& path) override;

    AndroidBitmapDataLoader& operator=(const AndroidBitmapDataLoader& other) = delete;
    AndroidBitmapDataLoader&& operator=(AndroidBitmapDataLoader&& other) = delete;
};


#endif //ANDROID_NDK_OPENGL_RESEARCH_ANDROIDBITMAPDATALOADER_H
