//
// Created by Igor Lapin on 06/08/2020.
//

#include <cmath>
#include "DisplayInfoUpdateDetector.h"

bool DisplayInfoUpdateDetector::isDisplayInfoUpdated() {
    auto currentDisplayWidth = m_displayInfo->width();
    auto currentDisplayHeight = m_displayInfo->height();
    auto currentDisplayDensityFactor = m_displayInfo->densityFactor();

    auto isDisplayInfoUpdated =
            std::isnan(m_lastDisplayWidth) ||
            std::isnan(m_lastDisplayHeight) ||
            std::isnan(m_lastDisplayDensityFactor) ||
            abs(currentDisplayWidth - m_lastDisplayWidth) > FLT_EPSILON ||
            abs(currentDisplayHeight - m_lastDisplayHeight) > FLT_EPSILON ||
            abs(currentDisplayDensityFactor - m_lastDisplayDensityFactor) > FLT_EPSILON;

    m_lastDisplayWidth = currentDisplayWidth;
    m_lastDisplayHeight = currentDisplayHeight;
    m_lastDisplayDensityFactor = currentDisplayDensityFactor;

    return isDisplayInfoUpdated;
}
