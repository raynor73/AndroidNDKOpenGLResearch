//
// Created by Igor Lapin on 09/07/2020.
//

#ifndef ENGINE_3D_MATERIALCOMPONENT_H
#define ENGINE_3D_MATERIALCOMPONENT_H


#include <string>
#include <utility>
#include "Material.h"
#include "GameObjectComponent.h"

class MaterialComponent : public GameObjectComponent {

    Material m_material;

public:
    static const std::string TYPE_NAME;

    MaterialComponent(Material material) : m_material(std::move(material)) {}

    const Material& material() const { return m_material; }
    void setMaterial(const Material& material) { m_material = material; }

    virtual const std::string& typeName() const override { return TYPE_NAME; };
    virtual std::shared_ptr<GameObjectComponent> clone() override;
};


#endif //ENGINE_3D_MATERIALCOMPONENT_H
