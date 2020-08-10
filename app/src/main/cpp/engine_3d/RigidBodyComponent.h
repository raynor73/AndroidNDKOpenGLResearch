//
// Created by Igor Lapin on 10/08/2020.
//

#ifndef ENGINE_3D_RIGID_BODY_COMPONENT_H
#define ENGINE_3D_RIGID_BODY_COMPONENT_H


#include <string>
#include "GameObjectComponent.h"

class RigidBodyComponent : public GameObjectComponent {

public:
    static const std::string TYPE_NAME;

    virtual ~RigidBodyComponent() override;

    virtual void onGameObjectAttachedToParent() override;
    virtual void onGameObjectDetachedFromParent() override;

    virtual const std::string& typeName() const { return TYPE_NAME; }

    virtual std::shared_ptr <GameObjectComponent> clone();
};


#endif //ENGINE_3D_RIGID_BODY_COMPONENT_H
