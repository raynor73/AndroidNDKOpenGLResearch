//
// Created by Igor Lapin on 29/06/2020.
//

#ifndef ANDROID_NDK_OPENGL_RESEARCH_ANDROIDSCENEDATALOADER_H
#define ANDROID_NDK_OPENGL_RESEARCH_ANDROIDSCENEDATALOADER_H


#include <jni.h>
#include <engine_3d/SceneDataLoader.h>

class AndroidSceneDataLoader : public SceneDataLoader {

    JavaVM* m_javaVm;
    jclass m_sceneJsonStringLoaderClass;
    jobject m_sceneJsonStringLoaderObject;

public:
    AndroidSceneDataLoader(JavaVM* javaVm, jclass sceneJsonStringLoaderClass, jobject sceneJsonStringLoaderObject) :
        m_javaVm(javaVm),
        m_sceneJsonStringLoaderClass(sceneJsonStringLoaderClass),
        m_sceneJsonStringLoaderObject(sceneJsonStringLoaderObject) {}

    virtual void loadSceneData(const std::string &path, Scene &scene) override;
};


#endif //ANDROID_NDK_OPENGL_RESEARCH_ANDROIDSCENEDATALOADER_H
