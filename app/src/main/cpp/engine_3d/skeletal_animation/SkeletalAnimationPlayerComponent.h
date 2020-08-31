//
// Created by Igor Lapin on 23/08/2020.
//

#ifndef ENGINE_3D_SKELETAL_ANIMATION_PLAYER_COMPONENT_H
#define ENGINE_3D_SKELETAL_ANIMATION_PLAYER_COMPONENT_H


#include <vector>
#include <engine_3d/GameObjectComponent.h>
#include <engine_3d/Constants.h>
#include <glm/ext/matrix_float4x4.hpp>
#include <unordered_map>
#include <game/TimeProvider.h>
#include <engine_3d/Time.h>
#include "Joint.h"
#include "KeyFrame.h"

class SkeletalAnimationPlayerComponent : public GameObjectComponent {

    std::shared_ptr<Time> m_time;

    std::vector<glm::mat4> m_jointTransforms;
    std::unordered_map<std::string, glm::mat4> m_preCalculatedJointWorldTransform;

    float m_elapsedTime = 0;
    bool m_isPlaying = false;

public:
    static const std::string TYPE_NAME;

    SkeletalAnimationPlayerComponent(std::shared_ptr<Time> time);

    const std::vector<glm::mat4>& jointTransforms() const { return m_jointTransforms; }

    virtual void setEnabled(bool isEnabled) override;

    virtual void update() override;

    void play();
    void stop();

    virtual void onDetachedFromGameObject() override;

    virtual const std::string& typeName() const override { return TYPE_NAME; }
    virtual std::shared_ptr<GameObjectComponent> clone() override;

private:
    glm::mat4 calculateJointWorldTransform(const Joint& joint, const KeyFrame& keyFrame);
};


#endif //ENGINE_3D_SKELETAL_ANIMATION_PLAYER_COMPONENT_H
