//
// Created by Igor Lapin on 19/08/2020.
//

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <sstream>
#include <glm/ext.hpp>
#include <glm/gtx/quaternion.hpp>
#include <map>
#include "AndroidSkeletalAnimationLoadingRepository.h"
#include "L.h"

AnimatedMesh AndroidSkeletalAnimationLoadingRepository::loadAnimation(const std::string& path) {
    JNIEnv *env;
    m_javaVm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6);
    jmethodID method = env->GetMethodID(
            m_binaryDataLoaderClass,
            "loadBinaryFileFromAssets",
            "(Ljava/lang/String;)[B"
    );
    jstring pathJString = env->NewStringUTF(path.c_str());
    auto resultByteArray = static_cast<jbyteArray>(env->CallObjectMethod(
            m_binaryDataLoaderObject,
            method,
            pathJString
    ));
    jbyte* resultJBytes = env->GetByteArrayElements(resultByteArray, JNI_FALSE);
    jsize lengthOfResultArray = env->GetArrayLength(resultByteArray);

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFileFromMemory(
            static_cast<void*>(resultJBytes),
            lengthOfResultArray,
            aiProcess_Triangulate,
            nullptr
    );

    std::vector<Vertex> vertices;
    std::vector<uint16_t> indices;
    float animationLength;
    std::vector<KeyFrame> keyFrames;
    if (scene != nullptr) {
        if (scene->mNumAnimations != 1) {
            std::stringstream ss;
            ss << "No or more than one animation in " << path;
            throw std::domain_error(ss.str());
        }

        if (scene->mNumMeshes != 1) {
            std::stringstream ss;
            ss << "No or more than one animated mesh in " << path;
            throw std::domain_error(ss.str());
        }

        auto assimpAnimation = scene->mAnimations[0];
        auto ticksPerSecond = assimpAnimation->mTicksPerSecond;
        if (ticksPerSecond == 0) {
            std::stringstream ss;
            ss << "Skeletal animation " << path << " has not ticks-per-second information";
            throw std::domain_error(ss.str());
        }
        animationLength = assimpAnimation->mDuration / ticksPerSecond;
        std::map<float, KeyFrame> timeToKeyFrameMap;
        for (int i = 0; i < assimpAnimation->mNumChannels; i++) {
            auto assimpNodeInfo = assimpAnimation->mChannels[i];
            if (
                    assimpNodeInfo->mNumPositionKeys != assimpNodeInfo->mNumRotationKeys ||
                    assimpNodeInfo->mNumScalingKeys != assimpNodeInfo->mNumRotationKeys
            ) {
                std::stringstream ss;
                ss << "Unequal number of transform keys: position keys: " << assimpNodeInfo->mNumPositionKeys <<
                    "; rotation keys: " << assimpNodeInfo->mNumPositionKeys <<
                    "; scale keys: " << assimpNodeInfo->mNumScalingKeys;
                throw std::domain_error(ss.str());
            }
            for (int j = 0; j < assimpNodeInfo->mNumPositionKeys; j++) {
                float time = assimpNodeInfo->mPositionKeys[j].mTime;

                if (!isFloatsEqual(time, assimpNodeInfo->mRotationKeys[j].mTime)) {
                    throw std::domain_error("Unequal time of rotation and position key frames found");
                }
                if (!isFloatsEqual(time, assimpNodeInfo->mScalingKeys[j].mTime)) {
                    throw std::domain_error("Unequal time of scaling and position key frames found");
                }

                auto transform = glm::translate(
                        glm::identity<glm::mat4>(),
                        aiVector3DToGlmVec3(assimpNodeInfo->mPositionKeys[j].mValue)
                );
                transform *= glm::toMat4(aiQuaternionToGlmQuat(assimpNodeInfo->mRotationKeys[j].mValue));
                transform = glm::scale(transform, aiVector3DToGlmVec3(assimpNodeInfo->mScalingKeys[j].mValue));
                if (timeToKeyFrameMap.count(time) > 0) {
                    timeToKeyFrameMap.at(time).jointLocalTransform.insert(
                            { std::string(assimpNodeInfo->mNodeName.C_Str()), transform }
                    );
                } else {
                    KeyFrame keyFrame;
                    keyFrame.time = time;
                    keyFrame.jointLocalTransform.insert({ std::string(assimpNodeInfo->mNodeName.C_Str()), transform });
                    timeToKeyFrameMap.insert({ time, keyFrame });
                }
            }
        }
        for (auto& keyFrame : timeToKeyFrameMap) {
            keyFrames.push_back(keyFrame.second);
        }
        std::stringstream ss;
        ss << "Number of key frames: " << keyFrames.size();
        L::d("!@£", ss.str());
        /*if (scene->mNumMeshes > 0) {
            auto assimpMesh = scene->mMeshes[0];

            if (assimpMesh->mNumVertices == 0) {
                std::stringstream ss;
                ss << "No vertices found for mesh " << assimpMesh->mName.C_Str() << " in " << path;
                throw std::domain_error(ss.str());
            }
            if (assimpMesh->mNumFaces == 0) {
                std::stringstream ss;
                ss << "No faces found for mesh " << assimpMesh->mName.C_Str() << " in " << path;
                throw std::domain_error(ss.str());
            }

            for (int faceIndex = 0; faceIndex < assimpMesh->mNumFaces; faceIndex++) {
                auto face = assimpMesh->mFaces[faceIndex];

                if (face.mNumIndices != 3) {
                    std::stringstream ss;
                    ss << "Wrong indices count: " << face.mNumIndices << " for one of face of mesh \"" << assimpMesh->mName.C_Str() << "\" in " << path;
                    throw std::domain_error(ss.str());
                }

                for (int i = 0; i < face.mNumIndices; i++) {
                    auto index = face.mIndices[i];

                    auto assimpVertex = assimpMesh->mVertices[index];
                    auto assimpNormal = assimpMesh->mNormals[index];
                    auto assimpUv = assimpMesh->mTextureCoords[0][index];

                    Vertex vertex {
                            glm::vec3 { assimpVertex.x, assimpVertex.y, assimpVertex.z },
                            glm::vec3 { assimpNormal.x, assimpNormal.y, assimpNormal.z },
                            glm::vec2 { assimpUv.x, assimpUv.y }
                    };

                    vertices.push_back(vertex);
                    indices.push_back(index);
                }
            }
        } else {
            std::stringstream ss;
            ss << "No meshes found in " << path;
            throw std::domain_error(ss.str());
        }*/
    } else {
        std::stringstream ss;
        ss << "Error importing skeletal animation " << path;
        throw std::domain_error(ss.str());
    }

    env->DeleteLocalRef(pathJString);
    env->ReleaseByteArrayElements(resultByteArray, resultJBytes, JNI_ABORT);

    return AnimatedMesh { Mesh { vertices, indices }, SkeletalAnimation { animationLength, keyFrames } };
}
