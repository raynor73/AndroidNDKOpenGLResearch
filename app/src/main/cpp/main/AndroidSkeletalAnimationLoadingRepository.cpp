//
// Created by Igor Lapin on 19/08/2020.
//

#include <sstream>
#include <glm/ext.hpp>
#include <glm/gtx/quaternion.hpp>
#include <map>
#include "AndroidSkeletalAnimationLoadingRepository.h"
#include "L.h"

SkeletalAnimation AndroidSkeletalAnimationLoadingRepository::loadAnimation(
        Mesh& animatedMesh,
        const std::string& path
) {
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
    importer.SetPropertyInteger(AI_CONFIG_PP_LBW_MAX_WEIGHTS, MAX_WEIGHTS);
    const aiScene* scene = importer.ReadFileFromMemory(
            static_cast<void*>(resultJBytes),
            lengthOfResultArray,
            aiProcess_Triangulate | aiProcess_LimitBoneWeights,
            nullptr
    );

    float animationLength;
    std::vector<KeyFrame> keyFrames;
    std::shared_ptr<Joint> rootJoint;
    std::vector<std::shared_ptr<Joint>> joints;
    if (scene != nullptr) {
        if (scene->mNumAnimations != 1) {
            std::stringstream ss;
            ss << "No or more than one animation in " << path;
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
        {
            std::stringstream ss;
            ss << "Number of key frames: " << keyFrames.size();
            L::d("!@£", ss.str());
        }

        if (scene->mNumMeshes == 1) {
            auto assimpMesh = scene->mMeshes[0];
            joints.resize(assimpMesh->mNumBones);
            {
                std::stringstream ss;
                ss << "Number of bones: " << assimpMesh->mNumBones;
                L::d("!@£", ss.str());
            }
            std::multimap<int, std::pair<int, float>> vertexIndexToJointIndexAndWeightsMap;
            std::unordered_map<std::string, int> boneNameToJointIndexMap;
            std::unordered_map<std::string, aiBone*> boneNameToBoneMap;
            std::unordered_map<std::string, std::shared_ptr<Joint>> jointNameToJointMap;
            for (int i = 0; i < assimpMesh->mNumBones; i++) {
                auto bone = assimpMesh->mBones[i];
                boneNameToBoneMap.insert({ bone->mName.C_Str(), bone });
                boneNameToJointIndexMap.insert({ bone->mName.C_Str(), i });
                for (int j = 0; j < bone->mNumWeights; j++) {
                    auto weightInfo = bone->mWeights[j];
                    vertexIndexToJointIndexAndWeightsMap.insert({ weightInfo.mVertexId, { i, weightInfo.mWeight} });
                }
            }
            for (auto& [name, bone] : boneNameToBoneMap) {
                L::d("!@£", name);
            }

            auto rootBoneNode = findRootBoneNode(scene->mRootNode, boneNameToBoneMap);
            if (rootBoneNode == nullptr) {
                throw std::domain_error("Root bone node not found");
            }
            std::string rootJointName = rootBoneNode->mName.C_Str();
            rootJoint = std::make_shared<Joint>(
                    rootJointName,
                    aiMatrix4x4toGlmMat4(boneNameToBoneMap.at(rootJointName)->mOffsetMatrix)
            );
            buildJointHierarchyFromNodes(rootBoneNode, rootJoint, boneNameToBoneMap, jointNameToJointMap);
            printJointHierarchy(rootJoint);
            for (auto& [jointName, joint] : jointNameToJointMap) {
                joints[boneNameToJointIndexMap.at(jointName)] = joint;
            }

            for (
                    auto vertexIndexIterator = vertexIndexToJointIndexAndWeightsMap.begin();
                    vertexIndexIterator != vertexIndexToJointIndexAndWeightsMap.end();
                    vertexIndexIterator = vertexIndexToJointIndexAndWeightsMap.upper_bound(vertexIndexIterator->first)
            ) {
                if (vertexIndexToJointIndexAndWeightsMap.count(vertexIndexIterator->first) > MAX_WEIGHTS) {
                    std::stringstream ss;
                    ss << "Maximum " << MAX_WEIGHTS << " weight(s) allowed per vertex but " << vertexIndexToJointIndexAndWeightsMap.count(vertexIndexIterator->first) << " found in " << path;
                    throw std::domain_error(ss.str());
                }
                glm::vec3 glmVec3Weights;
                glm::ivec3 glmIvec3indices;
                auto weightIterators = vertexIndexToJointIndexAndWeightsMap.equal_range(vertexIndexIterator->first);
                for (
                        auto [i, it] = std::tuple { 0, weightIterators.first };
                        it != weightIterators.second;
                        i++, it++
                ) {
                    glmVec3Weights[i] = it->second.second;
                    glmIvec3indices[i] = it->second.first;
                }
                animatedMesh.setJointWeights(vertexIndexIterator->first, glmVec3Weights);
                animatedMesh.setJointIndices(vertexIndexIterator->first, glmIvec3indices);
            }
        } else {
            std::stringstream ss;
            ss << "Number of meshes found in " << path << " is: " << scene->mNumMeshes << " but should be exactly 1";
            throw std::domain_error(ss.str());
        }
    } else {
        std::stringstream ss;
        ss << "Error importing skeletal animation " << path;
        throw std::domain_error(ss.str());
    }

    env->DeleteLocalRef(pathJString);
    env->ReleaseByteArrayElements(resultByteArray, resultJBytes, JNI_ABORT);

    return SkeletalAnimation { animationLength, rootJoint, joints, keyFrames };
}

void AndroidSkeletalAnimationLoadingRepository::buildJointHierarchyFromNodes(
        aiNode* node,
        std::shared_ptr<Joint>& joint,
        const std::unordered_map<std::string, aiBone*>& boneNameToBoneMap,
        std::unordered_map<std::string, std::shared_ptr<Joint>>& jointNameToJointMap
) {
    for (int i = 0; i < node->mNumChildren; i++) {
        auto childNode = node->mChildren[i];
        if (boneNameToBoneMap.count(childNode->mName.C_Str()) == 0) {
            continue;
        }

        auto childBone = boneNameToBoneMap.at(childNode->mName.C_Str());
        auto childJoint = std::make_shared<Joint>(
                childNode->mName.C_Str(),
                aiMatrix4x4toGlmMat4(childBone->mOffsetMatrix)
        );
        joint->addChild(childJoint);
        jointNameToJointMap.insert({ childJoint->name(), childJoint });

        buildJointHierarchyFromNodes(childNode, childJoint, boneNameToBoneMap, jointNameToJointMap);
    }
}

void AndroidSkeletalAnimationLoadingRepository::printJointHierarchy(const std::shared_ptr<Joint>& joint, int level) {
    std::stringstream ss;
    for (int i = 0; i < level; i++) {
        ss << "    ";
    }
    ss << joint->name();
    L::d("!@£", ss.str());
    for (auto& child : joint->children()) {
        printJointHierarchy(child, level + 1);
    }
}

aiNode* AndroidSkeletalAnimationLoadingRepository::findRootBoneNode(
        aiNode* node,
        const std::unordered_map<std::string, aiBone*>& boneNameToBoneMap
) {
    std::string nodeName = node->mName.C_Str();
    if (boneNameToBoneMap.count(nodeName) > 0) {
        return node;
    }
    
    for (int i = 0; i < node->mNumChildren; i++) {
        auto nodeCandidate = findRootBoneNode(node->mChildren[i], boneNameToBoneMap);
        if (nodeCandidate != nullptr) {
            return nodeCandidate;
        }
    }

    return nullptr;
}
