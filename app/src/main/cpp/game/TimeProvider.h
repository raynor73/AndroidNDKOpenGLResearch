//
// Created by Igor Lapin on 06/06/2020.
//

#ifndef GAME_TIMEPROVIDER_H
#define GAME_TIMEPROVIDER_H


#include <cstdint>

class TimeProvider {

public:
    int64_t getTimestamp();

    static const float NANOS_IN_SECOND;
};


#endif //GAME_TIMEPROVIDER_H
