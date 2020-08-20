//
// Created by Igor Lapin on 19/08/2020.
//

#ifndef GAME_SKELETAL_ANIMATION_LOADING_REPOSITORY_H
#define GAME_SKELETAL_ANIMATION_LOADING_REPOSITORY_H


#include <engine_3d/Mesh.h>
#include <engine_3d/skeletal_animation/SkeletalAnimation.h>

struct AnimatedMesh {
    Mesh mesh;
    SkeletalAnimation animation;
};

class SkeletalAnimationLoadingRepository {

public:
    virtual ~SkeletalAnimationLoadingRepository() = default;

    virtual AnimatedMesh loadAnimation(const std::string& path) = 0;
};


#endif //GAME_SKELETAL_ANIMATION_LOADING_REPOSITORY_H
