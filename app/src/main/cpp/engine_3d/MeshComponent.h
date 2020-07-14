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
    std::string m_meshName;

public:
    static const std::string TYPE_NAME;

    MeshComponent(Mesh mesh, std::string meshName) : m_mesh(std::move(mesh)), m_meshName(std::move(meshName)) {}

    const Mesh& mesh() const { return m_mesh; }

    const std::string& meshName() const { return m_meshName; }

    virtual const std::string& typeName() const override { return TYPE_NAME; }
    virtual std::shared_ptr<GameObjectComponent> clone() override;
};


#endif //ENGINE_3D_MESHCOMPONENT_H
