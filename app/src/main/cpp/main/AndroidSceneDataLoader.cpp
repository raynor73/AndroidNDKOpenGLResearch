//
// Created by Igor Lapin on 29/06/2020.
//

#include <iostream>
#include <main/L.h>
#include "AndroidSceneDataLoader.h"

void AndroidSceneDataLoader::loadSceneData(const std::string &path, Scene &scene) {
    JNIEnv *env;
    m_javaVm->AttachCurrentThread(&env, nullptr);
    jmethodID method = env->GetMethodID(
            m_sceneJsonStringLoaderClass,
            "loadTextFileFromAssets",
            "(Ljava/lang/String;)Ljava/lang/String;"
    );
    auto resultJString = static_cast<jstring>(env->CallObjectMethod(m_sceneJsonStringLoaderObject, method));
    /*const char *resultCString = env->GetStringUTFChars(resultJString, JNI_FALSE);
    std::string jsonString = resultCString;
    L::d("!@£", jsonString);
    env->ReleaseStringUTFChars(resultJString, resultCString);*/
}
