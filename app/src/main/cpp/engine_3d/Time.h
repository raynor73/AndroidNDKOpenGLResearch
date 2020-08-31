//
// Created by Igor Lapin on 30/08/2020.
//

#ifndef ENGINE_3D_TIME_H
#define ENGINE_3D_TIME_H


#include <memory>
#include "TimeProvider.h"

class Time {

    std::shared_ptr<TimeProvider> m_timeProvider;

    float m_timeScale = 1;

    float m_deltaTime = 0;
    float m_unscaledDeltaTime = 0;

    float m_prevTimestamp = 0;
    bool m_hasPrevTimestamp = false;

public:
    Time(std::shared_ptr<TimeProvider> timeProvider);

    void update();

    float timeScale() const { return m_timeScale; }
    void setTimeScale(float scale) { m_timeScale = scale; }

    float deltaTime() const { return m_deltaTime; }

    float unscaledDeltaTime() const { return m_unscaledDeltaTime; }
};


#endif //ENGINE_3D_TIME_H
