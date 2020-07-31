//
// Created by Igor Lapin on 26/07/2020.
//

#include <memory>
#include "ViewBoundsComponent.h"

const std::string ViewBoundsComponent::TYPE_NAME = "ViewBoundsComponent";

std::shared_ptr<GameObjectComponent> ViewBoundsComponent::clone() {
    auto clone = std::make_shared<ViewBoundsComponent>(m_left, m_top, m_right, m_bottom);
    clone->setEnabled(m_isEnabled);
    return clone;
}
