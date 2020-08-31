//
// Created by Igor Lapin on 06/08/2020.
//

#ifndef GAME_DISPLAY_INFO_CHANGE_DETECTOR_H
#define GAME_DISPLAY_INFO_CHANGE_DETECTOR_H


#include <utility>
#include <memory>
#include "DisplayInfo.h"

class DisplayInfoUpdateDetector {

    float m_lastDisplayWidth = NAN;
    float m_lastDisplayHeight = NAN;
    float m_lastDisplayDensityFactor = NAN;

protected:
    std::shared_ptr<DisplayInfo> m_displayInfo;

public:
    DisplayInfoUpdateDetector(std::shared_ptr<DisplayInfo> displayInfo) : m_displayInfo(std::move(displayInfo)) {}
    virtual ~DisplayInfoUpdateDetector() = default;

    bool isDisplayInfoUpdated();
};


#endif //GAME_DISPLAY_INFO_CHANGE_DETECTOR_H
