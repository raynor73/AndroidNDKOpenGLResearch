//
// Created by Igor Lapin on 26/08/2020.
//

#include "AndroidReadOnlyFsAbstraction.h"

bool AndroidReadOnlyFsAbstraction::isFileExists(const std::string& path) {
    JNIEnv *env;
    m_javaVm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6);
    jmethodID method = env->GetMethodID(
            m_bridgeClass,
            "isFileExistsInPrivateStorage",
            "(Ljava/lang/String;)Z"
    );
    jstring pathJString = env->NewStringUTF(path.c_str());

    auto isFileExists = env->CallBooleanMethod(m_bridgeObject, method, pathJString);

    env->DeleteLocalRef(pathJString);

    return isFileExists;
}

std::vector<uint8_t> AndroidReadOnlyFsAbstraction::readBinaryFileContent(const std::string& path) {
    return std::vector<uint8_t>();
}

std::string AndroidReadOnlyFsAbstraction::readTextFileContent(const std::string& path) {
    return std::string();
}
