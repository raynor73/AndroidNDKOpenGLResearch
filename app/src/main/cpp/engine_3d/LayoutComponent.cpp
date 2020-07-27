//
// Created by Igor Lapin on 26/07/2020.
//

#include <memory>
#include <sstream>
#include <exception>
#include "GameObject.h"
#include "LayoutComponent.h"
#include "ViewBoundsComponent.h"
#include "TransformationComponent.h"

const std::string LayoutComponent::TYPE_NAME = "LayoutComponent";

void LayoutComponent::update() {
    GameObjectComponent::update();

    if (!m_isLayoutRequired || !m_isEnabled) {
        return;
    }
    m_isLayoutRequired = false;

    if (m_layoutParams.referenceViewBounds == nullptr) {
        throw std::domain_error("Can't layout without reference view bounds provided");
    }

    if (m_gameObject == nullptr) {
        throw std::domain_error("Layout component has no game object");
    }

    auto viewBounds = std::static_pointer_cast<ViewBoundsComponent>(
            m_gameObject->findComponent(ViewBoundsComponent::TYPE_NAME)
    );
    if (viewBounds == nullptr) {
        throw std::domain_error("No View Bounds to layout");
    }

    auto width = viewBounds->right() - viewBounds->left();
    auto height = viewBounds->top() - viewBounds->bottom();

    switch (m_layoutParams.horizontalLayoutType) {

        case HorizontalLayoutType::LEFT_INSIDE:
            viewBounds->setLeft(m_layoutParams.referenceViewBounds->left() + m_layoutParams.paddingLeft);
            viewBounds->setRight(viewBounds->left() + width);
            break;

        case HorizontalLayoutType::LEFT_OUTSIDE:
            viewBounds->setRight(m_layoutParams.referenceViewBounds->left() - m_layoutParams.paddingRight);
            viewBounds->setLeft(viewBounds->right() - width);
            break;

        case HorizontalLayoutType::CENTER: {
            auto referenceCenter =
                    (m_layoutParams.referenceViewBounds->left() + m_layoutParams.referenceViewBounds->right()) / 2;
            auto halfWidth = width / 2;
            viewBounds->setLeft(referenceCenter - halfWidth + m_layoutParams.paddingLeft);
            viewBounds->setRight(viewBounds->left() + width);
            break;
        }

        case HorizontalLayoutType::RIGHT_INSIDE:
            viewBounds->setRight(m_layoutParams.referenceViewBounds->right() - m_layoutParams.paddingRight);
            viewBounds->setLeft(viewBounds->right() - width);
            break;

        case HorizontalLayoutType::RIGHT_OUTSIDE:
            viewBounds->setLeft(m_layoutParams.referenceViewBounds->right() + m_layoutParams.paddingLeft);
            viewBounds->setRight(viewBounds->left() + width);
            break;
    }

    switch (m_layoutParams.verticalLayoutType) {

        case VerticalLayoutType::TOP_INSIDE:
            viewBounds->setTop(m_layoutParams.referenceViewBounds->top() - m_layoutParams.paddingTop);
            viewBounds->setBottom(viewBounds->top() - height);
            break;

        case VerticalLayoutType::TOP_OUTSIDE:
            viewBounds->setBottom(m_layoutParams.referenceViewBounds->bottom() + m_layoutParams.paddingBottom);
            viewBounds->setTop(viewBounds->bottom() + height);
            break;

        case VerticalLayoutType::CENTER: {
            auto referenceCenter =
                    (m_layoutParams.referenceViewBounds->top() + m_layoutParams.referenceViewBounds->bottom()) / 2;
            auto halfHeight = height / 2;
            viewBounds->setBottom(referenceCenter - halfHeight + m_layoutParams.paddingBottom);
            viewBounds->setTop(viewBounds->bottom() + height);
            break;
        }

        case VerticalLayoutType::BOTTOM_INSIDE:
            viewBounds->setBottom(m_layoutParams.referenceViewBounds->bottom() + m_layoutParams.paddingBottom);
            viewBounds->setTop(viewBounds->bottom() + height);
            break;

        case VerticalLayoutType::BOTTOM_OUTSIDE:
            viewBounds->setTop(m_layoutParams.referenceViewBounds->bottom() - m_layoutParams.paddingTop);
            viewBounds->setBottom(viewBounds->top() - height);
            break;
    }

    float originPositionY;
    switch (m_layoutParams.originVerticalLayoutType) {

        case OriginVerticalLayoutType::TOP:
            originPositionY = viewBounds->top();
            break;

        case OriginVerticalLayoutType::CENTER:
            originPositionY = (viewBounds->top() - viewBounds->bottom()) / 2;
            break;

        case OriginVerticalLayoutType::BOTTOM:
            originPositionY = viewBounds->bottom();
            break;
    }

    float originPositionX;
    switch (m_layoutParams.originHorizontalLayoutType) {

        case OriginHorizontalLayoutType::LEFT:
            originPositionX = viewBounds->left();
            break;

        case OriginHorizontalLayoutType::CENTER:
            originPositionX = (viewBounds->right() - viewBounds->left()) / 2;
            break;

        case OriginHorizontalLayoutType::RIGHT:
            originPositionX = viewBounds->right();
            break;
    }

    auto transform = std::static_pointer_cast<TransformationComponent>(
            m_gameObject->findComponent(TransformationComponent::TYPE_NAME)
    );
    if (transform == nullptr) {
        throw std::domain_error("No transform found for laying out game object");
    }

    auto originPosition = transform->position();
    originPosition.x = originPositionX;
    originPosition.y = originPositionY;
    transform->setPosition(originPosition);
}

std::shared_ptr <GameObjectComponent> LayoutComponent::clone() {
    auto clone = std::make_shared<LayoutComponent>(m_layoutParams);
    clone->setEnabled(m_isEnabled);
    return clone;
}

void LayoutComponent::setLayoutParams(const LayoutParams& layoutParams) {
    m_layoutParams = layoutParams;
    m_isLayoutRequired = true;
}
