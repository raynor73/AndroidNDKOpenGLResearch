//
// Created by Igor Lapin on 06/06/2020.
//

#ifndef GAME_TIMEPROVIDER_H
#define GAME_TIMEPROVIDER_H


#include <chrono>
#include <cstdint>

class TimeProvider {

    std::chrono::time_point<std::chrono::high_resolution_clock> m_startTimestamp;

public:
    TimeProvider();

    uint64_t getTimestamp();

    static const float NANOS_IN_SECOND;
};


#endif //GAME_TIMEPROVIDER_H
