//
// Created by Igor Lapin on 05/09/2020.
//

#include <main/L.h>
#include <main/Constants.h>
#include <glm/gtx/compatibility.hpp>
#include "ValueAnimator.h"

ValueAnimator::ValueAnimator(
        std::shared_ptr<Interpolator> interpolator,
        float start,
        float end,
        float duration,
        int repeatCount
) : m_interpolator(std::move(interpolator)),
    m_start(start),
    m_end(end),
    m_duration(duration),
    m_repeatCount(repeatCount)
{}

void ValueAnimator::start() {
    if (m_state != State::NOT_RUNNING) {
        L::e(App::Constants::LOG_TAG, "Animation is already running");
        return;
    }

    m_state = State::RUNNING;

    m_currentTime = 0;
    m_currentCount = 0;
    m_isCancelRequested = false;

    m_value = m_start;
}

void ValueAnimator::pause() {
    if (m_state != State::RUNNING) {
        L::e(App::Constants::LOG_TAG, "Animation is not running");
        return;
    }

    m_state = State::PAUSED;
}

void ValueAnimator::resume() {
    if (m_state != State::PAUSED) {
        L::e(App::Constants::LOG_TAG, "Animation is not paused");
        return;
    }

    m_state = State::RUNNING;
}

void ValueAnimator::cancel() {
    if (m_state == State::NOT_RUNNING) {
        L::e(App::Constants::LOG_TAG, "Animation is not running");
        return;
    }

    m_isCancelRequested = true;
}

void ValueAnimator::end() {
    if (m_state == State::NOT_RUNNING) {
        L::e(App::Constants::LOG_TAG, "Animation is not running");
        return;
    }

    m_currentTime = m_duration;
}

void ValueAnimator::update(float dt) {
    m_isEnded = false;
    m_isCancelled = false;

    if (m_state != State::RUNNING) {
        return;
    }

    auto newTime = m_currentTime + dt;
    if (newTime > m_duration) {
        if (m_repeatCount <= 0) {
            m_currentTime = fmod(newTime, m_duration);
        } else if (m_currentCount >= m_repeatCount - 1) {
            m_currentTime = m_duration;
            m_isEnded = true;
            m_state = State::NOT_RUNNING;
        } else {
            m_currentCount++;
            m_currentTime = fmod(newTime, m_duration);
        }
    } else {
        m_currentTime = newTime;
    }

    m_value = glm::lerp(m_start, m_end, m_interpolator->interpolate(m_currentTime / m_duration));

    if (m_isCancelRequested) {
        m_isCancelled = true;
        m_state = State::NOT_RUNNING;
    }
}
