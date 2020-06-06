//
// Created by Igor Lapin on 06/06/2020.
//

#ifndef GAME_MESSAGEQUEUE_H
#define GAME_MESSAGEQUEUE_H


#include <memory>
#include <queue>
#include <mutex>

class MessageQueue {

public:
    void putMessage(const std::shared_ptr<void> &message);
    void putMessageAndWaitForExecution(const std::shared_ptr<void> &message);
    void update();

    void setListener(void (*listener)(const std::shared_ptr<void> &message)) { m_listener = listener; }

private:
    std::queue<std::shared_ptr<void>> m_messageQueue;
    std::mutex m_mutex;
    void (*m_listener)(const std::shared_ptr<void> &message);
};


#endif //GAME_MESSAGEQUEUE_H
