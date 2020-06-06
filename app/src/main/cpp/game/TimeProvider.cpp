//
// Created by Igor Lapin on 06/06/2020.
//

#include <chrono>
#include "TimeProvider.h"

int64_t TimeProvider::getTimestamp() {
    return std::chrono::steady_clock::now().time_since_epoch().count();
}

const float TimeProvider::NANOS_IN_SECOND = 1e9f;
