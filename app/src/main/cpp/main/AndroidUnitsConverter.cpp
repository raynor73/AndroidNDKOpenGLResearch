//
// Created by Igor Lapin on 01/07/2020.
//

#include <glm/gtx/compatibility.hpp>
#include "AndroidUnitsConverter.h"

float AndroidUnitsConverter::widthPercentToPixels(float percentValue) {
    return glm::lerp(0.0f, m_displayInfo->width(), percentValue / 100);
}

float AndroidUnitsConverter::widthPixelsToPercent(float pixels) {
    return pixels / m_displayInfo->width() * 100;
}

float AndroidUnitsConverter::heightPercentToPixels(float percentValue) {
    return glm::lerp(0.0f, m_displayInfo->height(), percentValue / 100);
}

float AndroidUnitsConverter::heightPixelsToPercent(float pixels) {
    return pixels / m_displayInfo->height() * 100;
}

float AndroidUnitsConverter::dpToPixels(float dpValue) {
    return dpValue * m_displayInfo->densityFactor();
}

float AndroidUnitsConverter::pixelsToDp(float pixels) {
    return pixels / m_displayInfo->densityFactor();
}
