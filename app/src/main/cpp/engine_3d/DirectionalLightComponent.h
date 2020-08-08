//
// Created by Igor Lapin on 08/08/2020.
//

#ifndef ENGINE_3D_DIRECTIONAL_LIGHT_COMPONENT_H
#define ENGINE_3D_DIRECTIONAL_LIGHT_COMPONENT_H


#include <glm/vec3.hpp>
#include <vector>
#include <utility>
#include <string>
#include "GameObjectComponent.h"

class DirectionalLightComponent : public GameObjectComponent {

    glm::vec3 m_color;
    glm::vec3 m_direction;
    std::vector<std::string> m_layerNames;

public:
    static const std::string TYPE_NAME;

    DirectionalLightComponent(glm::vec3 color, glm::vec3 direction, std::vector<std::string> layerNames) :
        m_color(std::move(color)),
        m_direction(std::move(direction)),
        m_layerNames(std::move(layerNames))
    {}

    const glm::vec3& color() const { return m_color; }
    void setColor(glm::vec3 color) { m_color = std::move(color); }

    const glm::vec3& direction() const { return m_direction; }
    void setDirection(const glm::vec3& direction) { m_direction = direction; }

    const std::vector<std::string>& layerNames() const { return m_layerNames; }
    void setLayerNames(std::vector<std::string> layerNames) { m_layerNames = std::move(layerNames); }

    virtual const std::string &typeName() const override { return TYPE_NAME; }

    virtual std::shared_ptr<GameObjectComponent> clone() override;
};


#endif //ENGINE_3D_DIRECTIONAL_LIGHT_COMPONENT_H
