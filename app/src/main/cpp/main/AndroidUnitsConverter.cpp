//
// Created by Igor Lapin on 01/07/2020.
//

#include <glm/gtx/compatibility.hpp>
#include "AndroidUnitsConverter.h"

float AndroidUnitsConverter::complexValueToPixels(const ComplexValue& value) {
    if (std::holds_alternative<PercentValue>(value)) {
        auto& percentValue = std::get<PercentValue>(value);
        if (percentValue.dimensionType == DimensionType::WIDTH) {
            return glm::lerp(0.0f, m_displayInfo->width(), percentValue.value / 100);
        } else {
            return glm::lerp(0.0f, m_displayInfo->height(), percentValue.value / 100);
        }
    } else if (std::holds_alternative<DpValue>(value)) {
        auto& dpValue = std::get<DpValue>(value);
        return dpValue.value * m_displayInfo->densityFactor();
    } else {
        auto& plainValue = std::get<PlainValue>(value);
        return plainValue.value;
    }
}

ComplexValue AndroidUnitsConverter::widthPixelsToPercentValue(float pixels) {
    return PercentValue { DimensionType::WIDTH, pixels / m_displayInfo->width() * 100 };
}

ComplexValue AndroidUnitsConverter::heightPixelsToPercentValue(float pixels) {
    return PercentValue { DimensionType::HEIGHT, pixels / m_displayInfo->height() * 100 };
}

ComplexValue AndroidUnitsConverter::pixelsToDpValue(float pixels) {
    return PlainValue { pixels / m_displayInfo->densityFactor() };
}
