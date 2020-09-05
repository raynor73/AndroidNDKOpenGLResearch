//
// Created by Igor Lapin on 05/09/2020.
//

#ifndef ENGINE_3D_VALUE_ANIMATOR_H
#define ENGINE_3D_VALUE_ANIMATOR_H


#include <memory>
#include "Interpolator.h"

class ValueAnimator {

public:
    enum class State {
        NOT_RUNNING, RUNNING, PAUSED
    };

private:

    State m_state = State::NOT_RUNNING;
    bool m_isEnded = false;
    bool m_isCancelled = false;

    int m_currentCount = 0;
    float m_currentTime = 0;
    float m_value = 0;

    bool m_isCancelRequested = false;

    std::shared_ptr<Interpolator> m_interpolator;
    float m_start;
    float m_end;
    float m_duration;
    int m_repeatCount;

public:
    ValueAnimator(
            std::shared_ptr<Interpolator> interpolator,
            float start,
            float end,
            float duration,
            int repeatCount
    );

    void start();
    void pause();
    void resume();
    void cancel();
    void end();
    void update(float dt);

    State state() const { return m_state; }
    bool isEnded() const { return m_isEnded; }
    bool isCancelled() const { return m_isCancelled; }
    float value() const { return m_value; }
};


#endif //ENGINE_3D_VALUE_ANIMATOR_H
