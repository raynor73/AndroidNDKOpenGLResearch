//
// Created by Igor Lapin on 29/06/2020.
//

#include <iostream>
#include "AndroidSceneDataLoader.h"

void AndroidSceneDataLoader::loadSceneData(const std::string &path, Scene &scene) {
    JNIEnv *env;
    m_javaVm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6);
    jmethodID method = env->GetMethodID(
            m_sceneJsonStringLoaderClass,
            "loadTextFileFromAssets",
            "(Ljava/lang/String;)Ljava/lang/String;"
    );
    jstring pathJString = env->NewStringUTF(path.c_str());
    auto resultJString = static_cast<jstring>(env->CallObjectMethod(
            m_sceneJsonStringLoaderObject,
            method,
            pathJString
    ));
    const char* resultCString = env->GetStringUTFChars(resultJString, JNI_FALSE);
    std::string jsonString = resultCString;

    scene.buildHierarchyFromRepresentation(jsonString);

    env->ReleaseStringUTFChars(resultJString, resultCString);
}
