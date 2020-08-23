//
// Created by Igor Lapin on 23/08/2020.
//

#ifndef ENGINE_3D_SKELETAL_ANIMATION_COMPONENT_H
#define ENGINE_3D_SKELETAL_ANIMATION_COMPONENT_H


#include <string>
#include <engine_3d/GameObjectComponent.h>
#include "SkeletalAnimation.h"

class SkeletalAnimationComponent : public GameObjectComponent {

    SkeletalAnimation m_skeletalAnimation;
    std::string m_skeletalAnimationName;

public:
    static const std::string TYPE_NAME;

    SkeletalAnimationComponent(
            SkeletalAnimation skeletalAnimation,
            std::string skeletalAnimationName
    ) : m_skeletalAnimation(skeletalAnimation),
        m_skeletalAnimationName(skeletalAnimationName)
    {}

    const SkeletalAnimation& skeletalAnimation() const { return m_skeletalAnimation; }

    const std::string& skeletalAnimationName() const { return m_skeletalAnimationName; }

    virtual const std::string& typeName() const override { return TYPE_NAME; }

    virtual std::shared_ptr<GameObjectComponent> clone() override;
};


#endif //ENGINE_3D_SKELETAL_ANIMATION_COMPONENT_H
