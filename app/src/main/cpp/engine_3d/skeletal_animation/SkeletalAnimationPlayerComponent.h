//
// Created by Igor Lapin on 23/08/2020.
//

#ifndef ENGINE_3D_SKELETAL_ANIMATION_PLAYER_COMPONENT_H
#define ENGINE_3D_SKELETAL_ANIMATION_PLAYER_COMPONENT_H


#include <vector>
#include <engine_3d/GameObjectComponent.h>
#include <engine_3d/Constants.h>
#include <glm/ext/matrix_float4x4.hpp>

class SkeletalAnimationPlayerComponent : public GameObjectComponent {

    std::vector<glm::mat4> m_jointTransforms;

public:
    static const std::string TYPE_NAME;

    SkeletalAnimationPlayerComponent();

    const std::vector<glm::mat4>& jointTransforms() const { return m_jointTransforms; }

    virtual void update() override;

    void play();
    void stop();

    virtual void onDetachedFromGameObject() override;

    virtual const std::string& typeName() const override { return TYPE_NAME; }
    virtual std::shared_ptr<GameObjectComponent> clone() override;
};


#endif //ENGINE_3D_SKELETAL_ANIMATION_PLAYER_COMPONENT_H
