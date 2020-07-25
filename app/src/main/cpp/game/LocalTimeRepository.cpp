//
// Created by Igor Lapin on 25/07/2020.
//

#include <chrono>
#include "LocalTimeRepository.h"

uint64_t LocalTimeRepository::elapsedTime() {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - m_start).count();
}
