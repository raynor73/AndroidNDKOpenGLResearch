//
// Created by Igor Lapin on 14/08/2020.
//

#ifndef ENGINE_3D_COLLISIONS_INFO_COMPONENT_H
#define ENGINE_3D_COLLISIONS_INFO_COMPONENT_H


#include <vector>
#include <memory>
#include <glm/vec3.hpp>
#include "GameObjectComponent.h"

struct Collision {
    std::shared_ptr<GameObject> gameObject;
    glm::vec3 position;
    glm::vec3 normal;
    float depth;
};

class CollisionsInfoComponent : public GameObjectComponent {


public:
    static const std::string TYPE_NAME;

    std::vector<Collision> collisions;

    virtual const std::string& typeName() const override { return TYPE_NAME; }

    virtual std::shared_ptr<GameObjectComponent> clone() override;
};


#endif //ENGINE_3D_COLLISIONS_INFO_COMPONENT_H
