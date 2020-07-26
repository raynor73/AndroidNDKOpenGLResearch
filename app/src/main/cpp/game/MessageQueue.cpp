//
// Created by Igor Lapin on 06/06/2020.
//

#include "MessageQueue.h"

namespace MessageQueue {

    void Queue::putMessage(Message message) {
        const std::lock_guard<std::mutex> lock(m_mutex);

        m_messageQueue.push(message);
    }

    void Queue::putMessageAndWaitForExecution(Message message) {
        const std::lock_guard<std::mutex> lock(m_mutex);

        m_messageQueue.push(message);

        update();
    }

    void Queue::update() {
        const std::lock_guard<std::mutex> lock(m_mutex);

        while (!m_messageQueue.empty()) {
            auto message = m_messageQueue.front();
            if (m_listener != nullptr) {
                m_listener(message);
            }
            m_messageQueue.pop();
        }
    }
}


