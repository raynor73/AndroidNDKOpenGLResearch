//
// Created by Igor Lapin on 03/07/2020.
//

#ifndef ANDROID_NDK_OPENGL_RESEARCH_ANDROIDMESHLOADINGREPOSITORY_H
#define ANDROID_NDK_OPENGL_RESEARCH_ANDROIDMESHLOADINGREPOSITORY_H


#include <jni.h>
#include <game/MeshLoadingRepository.h>

class AndroidMeshLoadingRepository : public MeshLoadingRepository {

    JavaVM* m_javaVm;
    jclass m_binaryDataLoaderClass;
    jobject m_binaryDataLoaderObject;

public:
    AndroidMeshLoadingRepository(
            JavaVM* javaVm,
            jclass binaryDataLoaderClass,
            jobject binaryDataLoaderObject
    ) : m_javaVm(javaVm),
        m_binaryDataLoaderClass(binaryDataLoaderClass),
        m_binaryDataLoaderObject(binaryDataLoaderObject) {}
    AndroidMeshLoadingRepository(const AndroidMeshLoadingRepository& other) = delete;
    AndroidMeshLoadingRepository(AndroidMeshLoadingRepository&& other) = delete;

    virtual Mesh loadMesh(const std::string& path) override;

    AndroidMeshLoadingRepository& operator=(const AndroidMeshLoadingRepository& other) = delete;
    AndroidMeshLoadingRepository&& operator=(AndroidMeshLoadingRepository&& other) = delete;
};


#endif //ANDROID_NDK_OPENGL_RESEARCH_ANDROIDMESHLOADINGREPOSITORY_H
