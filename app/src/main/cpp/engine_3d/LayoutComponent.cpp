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

    if (!m_isEnabled) {
        return;
    }

    if (m_layoutParams.referenceViewBounds == nullptr) {
        throw std::domain_error("Can't layout without reference view bounds provided");
    }

    int currentReferenceLeftViewBound = m_layoutParams.referenceViewBounds->left();
    int currentReferenceTopViewBound = m_layoutParams.referenceViewBounds->top();
    int currentReferenceRightViewBound = m_layoutParams.referenceViewBounds->right();
    int currentReferenceBottomViewBound = m_layoutParams.referenceViewBounds->bottom();
    if (
            currentReferenceLeftViewBound == m_prevReferenceLeftViewBound &&
            currentReferenceTopViewBound == m_prevReferenceTopViewBound &&
            currentReferenceRightViewBound == m_prevReferenceRightViewBound &&
            currentReferenceBottomViewBound == m_prevReferenceBottomViewBound
    ) {
        return;
    }
    m_prevReferenceLeftViewBound = currentReferenceLeftViewBound;
    m_prevReferenceTopViewBound = currentReferenceTopViewBound;
    m_prevReferenceRightViewBound = currentReferenceRightViewBound;
    m_prevReferenceBottomViewBound = currentReferenceBottomViewBound;

    throwErrorIfNoGameObject();

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
            viewBounds->setLeft(currentReferenceLeftViewBound + m_layoutParams.paddingLeft);
            viewBounds->setRight(viewBounds->left() + width);
            break;

        case HorizontalLayoutType::LEFT_OUTSIDE:
            viewBounds->setRight(currentReferenceLeftViewBound - m_layoutParams.paddingRight);
            viewBounds->setLeft(viewBounds->right() - width);
            break;

        case HorizontalLayoutType::CENTER: {
            auto referenceCenter = (currentReferenceLeftViewBound + currentReferenceRightViewBound) / 2;
            auto halfWidth = width / 2;
            viewBounds->setLeft(referenceCenter - halfWidth + m_layoutParams.paddingLeft);
            viewBounds->setRight(viewBounds->left() + width);
            break;
        }

        case HorizontalLayoutType::RIGHT_INSIDE:
            viewBounds->setRight(currentReferenceRightViewBound - m_layoutParams.paddingRight);
            viewBounds->setLeft(viewBounds->right() - width);
            break;

        case HorizontalLayoutType::RIGHT_OUTSIDE:
            viewBounds->setLeft(currentReferenceRightViewBound + m_layoutParams.paddingLeft);
            viewBounds->setRight(viewBounds->left() + width);
            break;
    }

    switch (m_layoutParams.verticalLayoutType) {

        case VerticalLayoutType::TOP_INSIDE:
            viewBounds->setTop(currentReferenceTopViewBound - m_layoutParams.paddingTop);
            viewBounds->setBottom(viewBounds->top() - height);
            break;

        case VerticalLayoutType::TOP_OUTSIDE:
            viewBounds->setBottom(currentReferenceBottomViewBound + m_layoutParams.paddingBottom);
            viewBounds->setTop(viewBounds->bottom() + height);
            break;

        case VerticalLayoutType::CENTER: {
            auto referenceCenter = (currentReferenceTopViewBound + currentReferenceBottomViewBound) / 2;
            auto halfHeight = height / 2;
            viewBounds->setBottom(referenceCenter - halfHeight + m_layoutParams.paddingBottom);
            viewBounds->setTop(viewBounds->bottom() + height);
            break;
        }

        case VerticalLayoutType::BOTTOM_INSIDE:
            viewBounds->setBottom(currentReferenceBottomViewBound + m_layoutParams.paddingBottom);
            viewBounds->setTop(viewBounds->bottom() + height);
            break;

        case VerticalLayoutType::BOTTOM_OUTSIDE:
            viewBounds->setTop(currentReferenceBottomViewBound - m_layoutParams.paddingTop);
            viewBounds->setBottom(viewBounds->top() - height);
            break;
    }

    float originPositionY;
    switch (m_layoutParams.originVerticalLayoutType) {

        case OriginVerticalLayoutType::TOP:
            originPositionY = viewBounds->top();
            break;

        case OriginVerticalLayoutType::CENTER:
            originPositionY = (viewBounds->top() + viewBounds->bottom()) / 2;
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
            originPositionX = (viewBounds->right() + viewBounds->left()) / 2;
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
