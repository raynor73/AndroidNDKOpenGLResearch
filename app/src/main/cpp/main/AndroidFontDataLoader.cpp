//
// Created by Igor Lapin on 20/07/2020.
//

#include "AndroidFontDataLoader.h"

std::vector<uint8_t> AndroidFontDataLoader::loadFontData(const std::string& fontPath) {
    JNIEnv *env;
    m_javaVm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6);
    jmethodID method = env->GetMethodID(
            m_binaryDataLoaderClass,
            "loadBinaryFileFromAssets",
            "(Ljava/lang/String;)[B"
    );
    jstring pathJString = env->NewStringUTF(fontPath.c_str());
    auto resultByteArray = static_cast<jbyteArray>(env->CallObjectMethod(
            m_binaryDataLoaderObject,
            method,
            pathJString
    ));
    jbyte* resultJBytes = env->GetByteArrayElements(resultByteArray, JNI_FALSE);
    jsize lengthOfResultArray = env->GetArrayLength(resultByteArray);

    std::vector<uint8_t> fontData(lengthOfResultArray);
    std::memcpy(fontData.data(), resultJBytes, lengthOfResultArray);

    env->ReleaseByteArrayElements(resultByteArray, resultJBytes, JNI_ABORT);

    return fontData;
}
