//
// Created by Igor Lapin on 25/07/2020.
//

#ifndef ENGINE_3D_TIME_REPOSITORY_H
#define ENGINE_3D_TIME_REPOSITORY_H


#include <cstdint>

class TimeRepository
{
public:
    virtual ~TimeRepository() = default;

    virtual uint64_t elapsedTime() = 0;
};


#endif //ENGINE_3D_TIME_REPOSITORY_H
