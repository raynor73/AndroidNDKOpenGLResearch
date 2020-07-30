//
// Created by Igor Lapin on 26/07/2020.
//

#include <memory>
#include "GameObject.h"
#include "ViewBoundsComponent.h"
#include "LayoutComponent.h"

const std::string ViewBoundsComponent::TYPE_NAME = "ViewBoundsComponent";

std::shared_ptr<GameObjectComponent> ViewBoundsComponent::clone() {
    auto clone = std::make_shared<ViewBoundsComponent>(m_left, m_top, m_right, m_bottom);
    clone->setEnabled(m_isEnabled);
    return clone;
}

void ViewBoundsComponent::setLeft(int left) {
    m_left = left;
    requestLayoutIfNecessary();
}

void ViewBoundsComponent::setTop(int top) {
    m_top = top;
    requestLayoutIfNecessary();
}

void ViewBoundsComponent::setRight(int right) {
    m_right = right;
    requestLayoutIfNecessary();
}

void ViewBoundsComponent::setBottom(int bottom) {
    m_bottom = bottom;
    requestLayoutIfNecessary();
}

void ViewBoundsComponent::requestLayoutIfNecessary() {
    if (m_gameObject == nullptr) {
        throw std::domain_error("View bounds have no game object while attempt to search for layout component");
    }

    auto layoutComponent = std::static_pointer_cast<LayoutComponent>(
            m_gameObject->findComponent(LayoutComponent::TYPE_NAME)
    );
    if (layoutComponent != nullptr) {
        layoutComponent->requestLayout();
    }
}
