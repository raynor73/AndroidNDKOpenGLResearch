//
// Created by Igor Lapin on 24/08/2020.
//

#include "AndroidFsAbstraction.h"

bool AndroidFsAbstraction::isFileExists(const std::string& path) {
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

std::vector<uint8_t> AndroidFsAbstraction::readBinaryFileContent(const std::string& path) {
    JNIEnv *env;
    m_javaVm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6);
    jmethodID method = env->GetMethodID(
            m_bridgeClass,
            "loadBinaryFileFromPrivateStorage",
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

void AndroidFsAbstraction::writeBinaryFileContent(const std::string& path, const std::vector<uint8_t>& data) {
    JNIEnv *env;
    m_javaVm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6);
    jmethodID method = env->GetMethodID(
            m_bridgeClass,
            "saveDataToFileInPrivateStorage",
            "(Ljava/lang/String;[B)V"
    );
    jstring pathJString = env->NewStringUTF(path.c_str());

    jbyteArray dataJByteArray = env->NewByteArray(data.size());
    jbyte* dataJBytes = env->GetByteArrayElements(dataJByteArray, JNI_FALSE);
    std::memcpy(dataJBytes, data.data(), data.size());
    env->ReleaseByteArrayElements(dataJByteArray, dataJBytes, 0);

    env->CallVoidMethod(m_bridgeObject, method, pathJString, dataJByteArray);

    env->DeleteLocalRef(pathJString);
    env->DeleteLocalRef(dataJByteArray);
}

std::string AndroidFsAbstraction::readTextFileContent(const std::string& path) {
    JNIEnv *env;
    m_javaVm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6);
    jmethodID method = env->GetMethodID(
            m_bridgeClass,
            "loadTextFileFromPrivateStorage",
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

void AndroidFsAbstraction::writeTextFileContent(const std::string& path, const std::string& text) {
    JNIEnv *env;
    m_javaVm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6);
    jmethodID method = env->GetMethodID(
            m_bridgeClass,
            "saveTextToFileInPrivateStorage",
            "(Ljava/lang/String;Ljava/lang/String;)V"
    );
    jstring pathJString = env->NewStringUTF(path.c_str());
    jstring textJString = env->NewStringUTF(text.c_str());

    env->CallVoidMethod(m_bridgeObject, method, pathJString, textJString);

    env->DeleteLocalRef(pathJString);
    env->DeleteLocalRef(textJString);
}

void AndroidFsAbstraction::deleteFile(const std::string& path) {
    JNIEnv *env;
    m_javaVm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6);
    jmethodID method = env->GetMethodID(
            m_bridgeClass,
            "deleteFileFromPrivateStorage",
            "(Ljava/lang/String;)V"
    );
    jstring pathJString = env->NewStringUTF(path.c_str());

    env->CallVoidMethod(m_bridgeObject, method, pathJString);

    env->DeleteLocalRef(pathJString);
}
