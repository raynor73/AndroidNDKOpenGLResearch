//
// Created by Igor Lapin on 03/07/2020.
//

#ifndef GAME_MESHSTORAGE_H
#define GAME_MESHSTORAGE_H


#include <unordered_map>
#include <string>
#include <engine_3d/Mesh.h>

class MeshStorage {

    std::unordered_map<std::string, Mesh> m_storage;

public:
    MeshStorage() = default;
    MeshStorage(const MeshStorage& other) = delete;
    MeshStorage(MeshStorage&& other) = delete;

    void putMesh(const std::string& name, Mesh mesh);
    Mesh getMesh(const std::string& name);
    void removeMesh(const std::string& name);
    void removeAllMeshes();

    MeshStorage& operator=(const MeshStorage& other) = delete;
    MeshStorage& operator=(MeshStorage&& other) = delete;
};


#endif //GAME_MESHSTORAGE_H
