//
// Created by Igor Lapin on 01/08/2020.
//

#ifndef ENGINE_3D_TEXT_BUTTON_COMPONENT_H
#define ENGINE_3D_TEXT_BUTTON_COMPONENT_H


#include <glm/vec4.hpp>
#include <utility>
#include <string>
#include "GameObjectComponent.h"

class TextButtonComponent : public GameObjectComponent {

    glm::vec4 m_textColor;
    glm::vec4 m_pressedTextColor;

public:
    TextButtonComponent(
            glm::vec4 textColor,
            glm::vec4 pressedTextColor
    ) : m_textColor(std::move(textColor)),
        m_pressedTextColor(std::move(pressedTextColor)) {}

    virtual void update() override;

    virtual const std::string& typeName() const override { return TYPE_NAME; }

    virtual std::shared_ptr<GameObjectComponent> clone() override;

    static const std::string TYPE_NAME;
};


#endif //ENGINE_3D_TEXT_BUTTON_COMPONENT_H
