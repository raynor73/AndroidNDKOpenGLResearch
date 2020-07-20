//
// Created by Igor Lapin on 18/07/2020.
//

#ifndef ENGINE_3D_TEXT_COMPONENT_H
#define ENGINE_3D_TEXT_COMPONENT_H


#include <string>
#include <utility>
#include <glm/vec4.hpp>
#include "TextAppearance.h"
#include "GameObjectComponent.h"

class TextComponent : public GameObjectComponent {

    std::string m_text;
    glm::vec4 m_textColor;
    TextAppearance m_textAppearance;

public:
    static const std::string TYPE_NAME;

    TextComponent(
            std::string text,
            glm::vec4 textColor,
            TextAppearance textAppearance
    ) : m_text(std::move(text)),
        m_textColor(std::move(textColor)),
        m_textAppearance(std::move(textAppearance)) {}

    const std::string& text() const { return m_text; }
    void setText(const std::string& text) { m_text = text; }

    const glm::vec4& textColor() const { return m_textColor; }
    void setTextColor(const glm::vec4& textColor) { m_textColor = textColor; }

    const TextAppearance& textAppearance() const { return m_textAppearance; }
    void setTextAppearance(const TextAppearance& textAppearance) { m_textAppearance = textAppearance; }

    virtual const std::string& typeName() const override { return TYPE_NAME; }
    virtual std::shared_ptr<GameObjectComponent> clone() override;
};


#endif //ENGINE_3D_TEXT_COMPONENT_H
