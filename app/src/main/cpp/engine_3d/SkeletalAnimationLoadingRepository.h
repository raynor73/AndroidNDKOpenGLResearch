//
// Created by Igor Lapin on 19/08/2020.
//

#ifndef GAME_SKELETAL_ANIMATION_LOADING_REPOSITORY_H
#define GAME_SKELETAL_ANIMATION_LOADING_REPOSITORY_H


#include <engine_3d/Mesh.h>
#include <engine_3d/skeletal_animation/SkeletalAnimation.h>

class SkeletalAnimationLoadingRepository {

public:
    virtual ~SkeletalAnimationLoadingRepository() = default;

    virtual SkeletalAnimation loadAnimation(Mesh& animatedMesh, const std::string& path) = 0;
};


#endif //GAME_SKELETAL_ANIMATION_LOADING_REPOSITORY_H
