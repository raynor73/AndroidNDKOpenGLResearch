//
// Created by Igor Lapin on 23/08/2020.
//

#ifndef GAME_SKELETAL_ANIMATION_STORAGE_H
#define GAME_SKELETAL_ANIMATION_STORAGE_H


#include <string>
#include <exception>
#include <unordered_map>
#include <optional>
#include <engine_3d/skeletal_animation/SkeletalAnimation.h>

class SkeletalAnimationStorage {

    std::unordered_map<std::string, SkeletalAnimation> m_animations;

public:
    SkeletalAnimationStorage() = default;
    SkeletalAnimationStorage(const SkeletalAnimationStorage&) = delete;
    SkeletalAnimationStorage(SkeletalAnimationStorage&&) = delete;

    void putAnimation(const std::string& name, const SkeletalAnimation& animation);
    std::optional<SkeletalAnimation> findAnimation(const std::string& name);
    void removeAnimation(const std::string& name);
    void removeAllAnimations();

    SkeletalAnimationStorage& operator=(const SkeletalAnimationStorage&) = delete;
    SkeletalAnimationStorage& operator=(SkeletalAnimationStorage&&) = delete;
};


#endif //GAME_SKELETAL_ANIMATION_STORAGE_H
