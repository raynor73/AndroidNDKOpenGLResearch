//
// Created by Igor Lapin on 06/06/2020.
//

#include "MessageQueue.h"

void MessageQueue::putMessage(const std::shared_ptr<void> &message) {
    const std::lock_guard<std::mutex> lock(m_mutex);

    m_messageQueue.push(message);
}

void MessageQueue::putMessageAndWaitForExecution(const std::shared_ptr<void> &message) {
    const std::lock_guard<std::mutex> lock(m_mutex);

    m_messageQueue.push(message);

    update();
}

void MessageQueue::update() {
    const std::lock_guard<std::mutex> lock(m_mutex);

    while (!m_messageQueue.empty()) {
        auto message = m_messageQueue.front();
        if (m_listener != nullptr) {
            m_listener(message);
        }
        m_messageQueue.pop();
    }
}

