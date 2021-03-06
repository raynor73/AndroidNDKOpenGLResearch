//
// Created by Igor Lapin on 02/08/2020.
//

#include "AndroidBitmapDataLoader.h"

BitmapInfo AndroidBitmapDataLoader::loadBitmap(const std::string& path) {
    JNIEnv *env;
    m_javaVm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6);
    jmethodID method = env->GetMethodID(
            m_binaryDataLoaderClass,
            "loadBitmapFileFromAssets",
            "(Ljava/lang/String;)Lilapin/opengl_research/BitmapInfo;"
    );
    jstring pathJString = env->NewStringUTF(path.c_str());
    auto result = env->CallObjectMethod(
            m_binaryDataLoaderObject,
            method,
            pathJString
    );

    jclass bitmapInfoClass = env->GetObjectClass(result);
    jfieldID dataFieldID = env->GetFieldID(bitmapInfoClass, "data", "[B");
    jfieldID widthFieldID = env->GetFieldID(bitmapInfoClass, "width", "I");
    jfieldID heightFieldID = env->GetFieldID(bitmapInfoClass, "height", "I");

    auto dataByteArray = static_cast<jbyteArray>(env->GetObjectField(result, dataFieldID));
    jbyte* dataJBytes = env->GetByteArrayElements(dataByteArray, JNI_FALSE);
    jsize lengthOfResultArray = env->GetArrayLength(dataByteArray);

    jint width = env->GetIntField(result, widthFieldID);
    jint height = env->GetIntField(result, heightFieldID);

    std::vector<uint8_t> bitmapData(lengthOfResultArray);
    std::memcpy(bitmapData.data(), dataJBytes, lengthOfResultArray);
    std::vector<uint8_t> flippedBitmapData(lengthOfResultArray);
    for (size_t y = 0; y < height; y++) {
        auto reversedY = height - y - 1;
        std::memcpy(
                flippedBitmapData.data() + reversedY * width * BYTES_PER_PIXEL,
                bitmapData.data() + y * width * BYTES_PER_PIXEL,
                width * BYTES_PER_PIXEL
        );
    }

    env->ReleaseByteArrayElements(dataByteArray, dataJBytes, JNI_ABORT);
    env->DeleteLocalRef(pathJString);

    return BitmapInfo { flippedBitmapData, uint(width), uint(height) };
}
