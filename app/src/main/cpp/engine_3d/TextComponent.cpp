//
// Created by Igor Lapin on 18/07/2020.
//

#include "TextComponent.h"

const std::string TextComponent::TYPE_NAME = "TextComponent";

std::shared_ptr<GameObjectComponent> TextComponent::clone() {
    auto clone = std::make_shared<TextComponent>(m_text, m_textColor, m_textAppearance);
    clone->setEnabled(m_isEnabled);
    return clone;
}
