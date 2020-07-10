//
// Created by Igor Lapin on 10/07/2020.
//

#include "AndroidShaderSourceRepository.h"

std::string AndroidShaderSourceRepository::loadShaderSource(const std::string& path) {
    JNIEnv *env;
    m_javaVm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6);
    jmethodID method = env->GetMethodID(
            m_textFileLoaderClass,
            "loadTextFileFromAssets",
            "(Ljava/lang/String;)Ljava/lang/String;"
    );
    jstring pathJString = env->NewStringUTF(path.c_str());
    auto resultJString = static_cast<jstring>(env->CallObjectMethod(
            m_textFileLoaderObject,
            method,
            pathJString
    ));
    const char* resultCString = env->GetStringUTFChars(resultJString, JNI_FALSE);
    std::string source = resultCString;
    env->ReleaseStringUTFChars(resultJString, resultCString);

    return source;
}
