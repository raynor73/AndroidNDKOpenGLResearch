//
// Created by Igor Lapin on 04/07/2020.
//

#ifndef ENGINE_3D_AMBIENTLIGHTCOMPONENT_H
#define ENGINE_3D_AMBIENTLIGHTCOMPONENT_H


#include <glm/vec3.hpp>
#include <vector>
#include <string>
#include <utility>
#include "GameObjectComponent.h"

class AmbientLightComponent : public GameObjectComponent {

    glm::vec3 m_color;
    std::vector<std::string> m_layerNames;

public:
    static const std::string TYPE_NAME;

    AmbientLightComponent(glm::vec3 color, std::vector<std::string> layerNames) :
        m_color(std::move(color)),
        m_layerNames(std::move(layerNames)) {}

    const glm::vec3& color() const { return m_color; }
    void setColor(glm::vec3 color) { m_color = std::move(color); }

    const std::vector<std::string>& layerNames() const { return m_layerNames; }
    void setLayerNames(std::vector<std::string> layerNames) { m_layerNames = std::move(layerNames); }

    virtual const std::string &typeName() const override { return TYPE_NAME; }

    virtual std::shared_ptr<GameObjectComponent> clone() override;
};


#endif //ENGINE_3D_AMBIENTLIGHTCOMPONENT_H
