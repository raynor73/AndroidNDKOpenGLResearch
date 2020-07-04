//
// Created by Igor Lapin on 04/07/2020.
//

#ifndef ENGINE_3D_MESHCOMPONENT_H
#define ENGINE_3D_MESHCOMPONENT_H


#include <utility>
#include <string>
#include "Mesh.h"
#include "GameObjectComponent.h"

class MeshComponent : public GameObjectComponent {

    Mesh m_mesh;

public:
    static const std::string TYPE_NAME;

    MeshComponent(Mesh mesh) : m_mesh(std::move(mesh)) {}

    virtual const std::string& typeName() const override { return TYPE_NAME; }
    virtual std::shared_ptr<GameObjectComponent> clone() override;
};


#endif //ENGINE_3D_MESHCOMPONENT_H
