//
// Created by Igor Lapin on 06/06/2020.
//

#include "AndroidDisplayInfo.h"

AndroidDisplayInfo::AndroidDisplayInfo(float initialWidth, float initialHeight, float initialDensityFactor) :
    m_width(initialWidth),
    m_height(initialHeight),
    m_densityFactor(initialDensityFactor)
{
    // do nothing
}

float AndroidDisplayInfo::width() const {
    return m_width;
}

float AndroidDisplayInfo::height() const {
    return m_height;
}

float AndroidDisplayInfo::densityFactor() const {
    return m_densityFactor;
}
