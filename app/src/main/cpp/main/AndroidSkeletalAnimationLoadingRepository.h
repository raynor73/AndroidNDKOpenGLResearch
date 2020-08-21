//
// Created by Igor Lapin on 19/08/2020.
//

#ifndef ANDROID_NDK_OPENGL_RESEARCH_ANDROIDSKELETALANIMATIONLOADINGREPOSITORY_H
#define ANDROID_NDK_OPENGL_RESEARCH_ANDROIDSKELETALANIMATIONLOADINGREPOSITORY_H


#include <game/SkeletalAnimationLoadingRepository.h>
#include <jni.h>
#include <assimp/Importer.hpp>

class AndroidSkeletalAnimationLoadingRepository : public SkeletalAnimationLoadingRepository {

    JavaVM* m_javaVm;
    jclass m_binaryDataLoaderClass;
    jobject m_binaryDataLoaderObject;

public:
    AndroidSkeletalAnimationLoadingRepository(
            JavaVM* javaVm,
            jclass binaryDataLoaderClass,
            jobject binaryDataLoaderObject
    ) : m_javaVm(javaVm),
        m_binaryDataLoaderClass(binaryDataLoaderClass),
        m_binaryDataLoaderObject(binaryDataLoaderObject) {}
    AndroidSkeletalAnimationLoadingRepository(const AndroidSkeletalAnimationLoadingRepository&) = delete;
    AndroidSkeletalAnimationLoadingRepository(AndroidSkeletalAnimationLoadingRepository&&) = delete;

    virtual AnimatedMesh loadAnimation(const std::string& path) override;

    AndroidSkeletalAnimationLoadingRepository& operator=(const AndroidSkeletalAnimationLoadingRepository&) = delete;
    AndroidSkeletalAnimationLoadingRepository& operator=(AndroidSkeletalAnimationLoadingRepository&&) = delete;

private:
    static constexpr int MAX_WEIGHTS = 3;

    inline static glm::vec3 aiVector3DToGlmVec3(const aiVector3D& vector) {
        return glm::vec3(vector.x, vector.y, vector.z);
    }

    inline static glm::quat aiQuaternionToGlmQuat(const aiQuaternion& quaternion) {
        return glm::quat(quaternion.w, quaternion.x, quaternion.y, quaternion.z);
    }

    inline static bool isFloatsEqual(float a, float b) {
        return std::abs(a - b) <= FLT_EPSILON;
    }
};


#endif //ANDROID_NDK_OPENGL_RESEARCH_ANDROIDSKELETALANIMATIONLOADINGREPOSITORY_H
