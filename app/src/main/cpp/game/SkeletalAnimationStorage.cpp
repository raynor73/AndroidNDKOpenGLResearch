//
// Created by Igor Lapin on 23/08/2020.
//

#include <sstream>
#include "SkeletalAnimationStorage.h"

void SkeletalAnimationStorage::putAnimation(
        const std::string& name,
        const SkeletalAnimation& animation
) {
    if (m_animations.count(name) > 0) {
        std::stringstream ss;
        ss << "Skeletal animation " << name << " already stored";
        throw std::domain_error(ss.str());
    }

    m_animations.insert({ name, animation });
}

std::optional<SkeletalAnimation> SkeletalAnimationStorage::findAnimation(const std::string& name) {
    if (m_animations.count(name) == 0) {
        return std::optional<SkeletalAnimation>();
    } else {
        return m_animations.at(name);
    }
}

void SkeletalAnimationStorage::removeAnimation(const std::string& name) {
    if (m_animations.count(name) == 0) {
        std::stringstream ss;
        ss << "No skeletal animation " << name << " to remove";
        throw std::domain_error(ss.str());
    }

    m_animations.erase(name);
}

void SkeletalAnimationStorage::removeAllAnimations() {
    m_animations.clear();
}
