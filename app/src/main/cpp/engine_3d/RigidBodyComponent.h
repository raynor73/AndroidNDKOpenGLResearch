//
// Created by Igor Lapin on 10/08/2020.
//

#ifndef ENGINE_3D_RIGID_BODY_COMPONENT_H
#define ENGINE_3D_RIGID_BODY_COMPONENT_H


#include <string>
#include <memory>
#include <utility>
#include "PhysicsEngine.h"
#include "GameObjectComponent.h"

class RigidBodyComponent : public GameObjectComponent {

    std::string m_rigidBodyName;
    std::shared_ptr<PhysicsEngine> m_physicsEngine;

public:
    static const std::string TYPE_NAME;

    RigidBodyComponent(
            std::string rigidBodyName,
            std::shared_ptr<PhysicsEngine> physicsEngine
    ) : m_rigidBodyName(std::move(rigidBodyName)),
        m_physicsEngine(std::move(physicsEngine))
    {}

    virtual ~RigidBodyComponent() override;

    virtual void onGameObjectAttachedToParent() override;
    virtual void onGameObjectDetachedFromParent() override;

    virtual void update() override;

    virtual const std::string& typeName() const override { return TYPE_NAME; }

    virtual std::shared_ptr <GameObjectComponent> clone() override;
};


#endif //ENGINE_3D_RIGID_BODY_COMPONENT_H
