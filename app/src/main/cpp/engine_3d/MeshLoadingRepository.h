//
// Created by Igor Lapin on 03/07/2020.
//

#ifndef GAME_MESHLOADINGREPOSITORY_H
#define GAME_MESHLOADINGREPOSITORY_H


#include <string>
#include <engine_3d/Mesh.h>

class MeshLoadingRepository {

public:
    virtual ~MeshLoadingRepository() = default;

    virtual Mesh loadMesh(const std::string& path) = 0;
};


#endif //GAME_MESHLOADINGREPOSITORY_H
