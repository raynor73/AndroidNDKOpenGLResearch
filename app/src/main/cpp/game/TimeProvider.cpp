//
// Created by Igor Lapin on 06/06/2020.
//

#include "TimeProvider.h"

TimeProvider::TimeProvider() {
    m_startTimestamp = std::chrono::high_resolution_clock::now();
}

int64_t TimeProvider::getTimestamp() {
    return (std::chrono::high_resolution_clock::now() - m_startTimestamp).count();
}

const float TimeProvider::NANOS_IN_SECOND = 1e9f;