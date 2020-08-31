//
// Created by Igor Lapin on 01/08/2020.
//

#include <memory>
#include "touch_screen/ClickDetectorComponent.h"
#include "Utils.h"
#include "GameObject.h"
#include "TextButtonComponent.h"
#include "TextComponent.h"

using namespace Engine3D::Utils;

const std::string TextButtonComponent::TYPE_NAME = "TextButtonComponent";

void TextButtonComponent::update() {
    GameObjectComponent::update();

    throwErrorIfNoGameObject();

    auto textComponent = std::static_pointer_cast<TextComponent>(
            m_gameObject->findComponent(TextComponent::TYPE_NAME)
    );
    throwErrorIfNull(textComponent, "Text component for text button not found");

    if (!m_isEnabled) {
        textComponent->setTextColor(m_textColor);
        return;
    }

    auto clickDetector = std::static_pointer_cast<ClickDetectorComponent>(
            m_gameObject->findComponent(ClickDetectorComponent::TYPE_NAME)
    );
    throwErrorIfNull(clickDetector, "Click detector for text button not found");

    if (clickDetector->isPressDetected()) {
        textComponent->setTextColor(m_pressedTextColor);
    } else {
        textComponent->setTextColor(m_textColor);
    }
}

std::shared_ptr<GameObjectComponent> TextButtonComponent::clone() {
    auto clone = std::make_shared<TextButtonComponent>(m_textColor, m_pressedTextColor);
    clone->setEnabled(m_isEnabled);
    return clone;
}

