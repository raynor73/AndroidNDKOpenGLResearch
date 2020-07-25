//
// Created by Igor Lapin on 25/07/2020.
//

#ifndef GAME_LOCAL_TIME_REPOSITORY_H
#define GAME_LOCAL_TIME_REPOSITORY_H

#include <chrono>
#include <engine_3d/TimeRepository.h>

class LocalTimeRepository : public TimeRepository {

    std::chrono::time_point<std::chrono::steady_clock> m_start;

public:
    LocalTimeRepository() : m_start(std::chrono::steady_clock::now()) {}

    virtual uint64_t elapsedTime() override;
};


#endif //GAME_LOCAL_TIME_REPOSITORY_H
