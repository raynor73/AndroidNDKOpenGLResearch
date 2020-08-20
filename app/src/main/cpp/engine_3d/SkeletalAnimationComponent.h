//
// Created by Igor Lapin on 19/08/2020.
//

#ifndef ENGINE_3D_SKELETAL_ANIMATION_COMPONENT_H
#define ENGINE_3D_SKELETAL_ANIMATION_COMPONENT_H


#include <utility>
#include "GameObjectComponent.h"
#include "skeletal_animation/SkeletalAnimation.h"

class SkeletalAnimationComponent : public GameObjectComponent {

    SkeletalAnimation m_animation;

public:
    static const std::string TYPE_NAME;

    SkeletalAnimationComponent(SkeletalAnimation animation) : m_animation(std::move(animation)) {}

    const SkeletalAnimation& animation() const { return m_animation; }
    void setAnimation(SkeletalAnimation animation) { m_animation = std::move(animation); };

    virtual const std::string& typeName() const override { return TYPE_NAME; }
    virtual std::shared_ptr<GameObjectComponent> clone() override;
};


#endif //ENGINE_3D_SKELETAL_ANIMATION_COMPONENT_H
