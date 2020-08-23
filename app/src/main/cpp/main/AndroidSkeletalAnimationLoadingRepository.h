//
// Created by Igor Lapin on 19/08/2020.
//

#ifndef ANDROID_NDK_OPENGL_RESEARCH_ANDROIDSKELETALANIMATIONLOADINGREPOSITORY_H
#define ANDROID_NDK_OPENGL_RESEARCH_ANDROIDSKELETALANIMATIONLOADINGREPOSITORY_H


#include <game/SkeletalAnimationLoadingRepository.h>
#include <jni.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

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

    virtual SkeletalAnimation loadAnimation(Mesh& animatedMesh, const std::string& path) override;

    AndroidSkeletalAnimationLoadingRepository& operator=(const AndroidSkeletalAnimationLoadingRepository&) = delete;
    AndroidSkeletalAnimationLoadingRepository& operator=(AndroidSkeletalAnimationLoadingRepository&&) = delete;

private:
    static constexpr int MAX_WEIGHTS = 3;

    static void buildJointHierarchyFromNodes(
            aiNode* node,
            std::shared_ptr<Joint>& joint,
            const std::unordered_map<std::string, aiBone*>& boneNameToBoneMap,
            std::unordered_map<std::string, std::shared_ptr<Joint>>& jointNameToJointMap
    );

    static aiNode* findRootBoneNode(aiNode* node, const std::unordered_map<std::string, aiBone*>& boneNameToBoneMap);

    static void printJointHierarchy(const std::shared_ptr<Joint>& joint, int level = 0);

    inline static glm::vec3 aiVector3DToGlmVec3(const aiVector3D& vector) {
        return glm::vec3(vector.x, vector.y, vector.z);
    }

    inline static glm::quat aiQuaternionToGlmQuat(const aiQuaternion& quaternion) {
        return glm::quat(quaternion.w, quaternion.x, quaternion.y, quaternion.z);
    }

    inline static glm::mat4 aiMatrix4x4toGlmMat4(const aiMatrix4x4& matrix) {
        glm::mat4 glmMatrix;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                glmMatrix[i][j] = matrix[i][j];
            }
        }
        return glmMatrix;
    }

    inline static bool isFloatsEqual(float a, float b) {
        return std::abs(a - b) <= FLT_EPSILON;
    }
};


#endif //ANDROID_NDK_OPENGL_RESEARCH_ANDROIDSKELETALANIMATIONLOADINGREPOSITORY_H
