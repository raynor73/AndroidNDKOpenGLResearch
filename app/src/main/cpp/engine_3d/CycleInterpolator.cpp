//
// Created by Igor Lapin on 05/09/2020.
//

#include <cmath>
#include "CycleInterpolator.h"

CycleInterpolator::CycleInterpolator(float period) : m_period(period) {}

float CycleInterpolator::interpolate(float input) {
    return 1 - (1 + cos(2 * M_PI * m_period * input)) / 2;
}
