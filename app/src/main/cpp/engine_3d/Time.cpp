//
// Created by Igor Lapin on 30/08/2020.
//

#include "Time.h"

Time::Time(std::shared_ptr<TimeProvider> timeProvider) : m_timeProvider(std::move(timeProvider)) {}

void Time::update() {
    auto currentTimestamp = m_timeProvider->getTimestamp();

    if (m_hasPrevTimestamp) {
        m_unscaledDeltaTime = (currentTimestamp - m_prevTimestamp) / TimeProvider::NANOS_IN_SECOND;
    } else {
        m_unscaledDeltaTime = 0.0f;
    }
    m_deltaTime = m_unscaledDeltaTime * m_timeScale;

    m_prevTimestamp = currentTimestamp;
    m_hasPrevTimestamp = true;
}
