//
// Created by Igor Lapin on 03/08/2020.
//

#ifndef ENGINE_3D_IMAGE_BUTTON_COMPONENT_H
#define ENGINE_3D_IMAGE_BUTTON_COMPONENT_H

#include <utility>
#include <string>
#include "Material.h"
#include "GameObjectComponent.h"

class ImageButtonComponent : public GameObjectComponent {

    Material m_material;
    Material m_pressedMaterial;

public:
    ImageButtonComponent(
            Material material,
            Material pressedMaterial
    ) : m_material(std::move(material)),
        m_pressedMaterial(std::move(pressedMaterial)) {}

    virtual void update() override;

    virtual const std::string& typeName() const override { return TYPE_NAME; }

    virtual std::shared_ptr<GameObjectComponent> clone() override;

    static const std::string TYPE_NAME;
};


#endif //ENGINE_3D_IMAGE_BUTTON_COMPONENT_H
