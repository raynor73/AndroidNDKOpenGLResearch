//
// Created by Igor Lapin on 26/08/2020.
//

#include <vector>
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
    JNIEnv *env;
    m_javaVm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6);
    jmethodID method = env->GetMethodID(
            m_bridgeClass,
            "loadBinaryFileFromAssets",
            "(Ljava/lang/String;)[B"
    );
    jstring pathJString = env->NewStringUTF(path.c_str());
    auto resultByteArray = static_cast<jbyteArray>(env->CallObjectMethod(m_bridgeObject, method, pathJString));
    jbyte* resultJBytes = env->GetByteArrayElements(resultByteArray, JNI_FALSE);
    jsize lengthOfResultArray = env->GetArrayLength(resultByteArray);

    std::vector<uint8_t> data(lengthOfResultArray);
    std::memcpy(data.data(), resultJBytes, lengthOfResultArray);

    env->ReleaseByteArrayElements(resultByteArray, resultJBytes, JNI_ABORT);
    env->DeleteLocalRef(pathJString);

    return data;
}

std::string AndroidReadOnlyFsAbstraction::readTextFileContent(const std::string& path) {
    JNIEnv *env;
    m_javaVm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6);
    jmethodID method = env->GetMethodID(
            m_bridgeClass,
            "loadTextFileFromAssets",
            "(Ljava/lang/String;)Ljava/lang/String;"
    );
    jstring pathJString = env->NewStringUTF(path.c_str());

    auto resultJString = static_cast<jstring>(env->CallObjectMethod(m_bridgeObject, method, pathJString));

    const char* resultCString = env->GetStringUTFChars(resultJString, JNI_FALSE);
    std::string text = resultCString;

    env->ReleaseStringUTFChars(resultJString, resultCString);
    env->DeleteLocalRef(pathJString);

    return text;
}
