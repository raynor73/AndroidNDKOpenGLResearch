//
// Created by Igor Lapin on 18/07/2020.
//

#ifndef ENGINE_3D_TEXT_COMPONENT_H
#define ENGINE_3D_TEXT_COMPONENT_H


#include <string>
#include <utility>
#include <glm/vec4.hpp>
#include "GameObjectComponent.h"

class TextComponent : public GameObjectComponent {

    std::string m_text;
    unsigned int m_textSize;
    glm::vec4 m_textColor;
    std::string m_fontName;

public:
    static const std::string TYPE_NAME;

    TextComponent(
            std::string text,
            unsigned int textSize,
            glm::vec4 textColor,
            std::string fontName
    ) : m_text(std::move(text)),
        m_textSize(textSize),
        m_textColor(std::move(textColor)),
        m_fontName(std::move(fontName)) {}

    const std::string& text() const { return m_text; }
    void setText(const std::string& text) { m_text = text; }

    unsigned int textSize() const { return m_textSize; }
    void setTextSize(unsigned int textSize) { m_textSize = textSize; }

    const glm::vec4& textColor() const { return m_textColor; }
    void setTextColor(const glm::vec4& textColor) { m_textColor = textColor; }

    const std::string& fontName() { return m_fontName; }
    void setFontName(const std::string& fontName) { m_fontName = fontName; }

    virtual const std::string& typeName() const override { return TYPE_NAME; }
    virtual std::shared_ptr<GameObjectComponent> clone() override;
};


#endif //ENGINE_3D_TEXT_COMPONENT_H
