//
// Created by Igor Lapin on 03/07/2020.
//

#include <sstream>
#include <exception>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <main/Constants.h>
#include <glm/vec3.hpp>
#include "AndroidMeshLoadingRepository.h"

Mesh AndroidMeshLoadingRepository::loadMesh(const std::string& path) {
    std::vector<Vertex> vertices;
    std::vector<uint16_t> indices;

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

    if (scene != nullptr) {
        if (scene->mNumMeshes > 0) {
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
                        glm::vec2 { assimpUv.x, assimpUv.y },
                        glm::ivec3(0),
                        glm::vec3(0)
                    };

                    vertices.push_back(vertex);
                    indices.push_back(index);
                }
            }
        } else {
            std::stringstream ss;
            ss << "No meshes found in " << path;
            throw std::domain_error(ss.str());
        }
    } else {
        std::stringstream ss;
        ss << "Error importing mesh " << path;
        throw std::domain_error(ss.str());
    }

    env->ReleaseByteArrayElements(resultByteArray, resultJBytes, JNI_ABORT);

    return Mesh { vertices, indices };
}
