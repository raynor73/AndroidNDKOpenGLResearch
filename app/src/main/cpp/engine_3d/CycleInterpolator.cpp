//
// Created by Igor Lapin on 05/09/2020.
//

#include <cmath>
#include "CycleInterpolator.h"

CycleInterpolator::CycleInterpolator(float period) : m_period(period) {}

float CycleInterpolator::interpolate(float input) {
    return sin(M_2_PI * m_period * input);
}
