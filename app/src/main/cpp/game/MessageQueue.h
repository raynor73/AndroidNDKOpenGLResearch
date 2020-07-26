//
// Created by Igor Lapin on 06/06/2020.
//

#ifndef GAME_MESSAGEQUEUE_H
#define GAME_MESSAGEQUEUE_H


#include <memory>
#include <queue>
#include <mutex>
#include <functional>

namespace MessageQueue {

    struct Message {
        std::string type;
        std::shared_ptr<void> data;
    };

    class Queue {

    public:
        void putMessage(Message message);
        void putMessageAndWaitForExecution(Message message);
        void update();

        void setListener(const std::function<void(Message)>& listener) { m_listener = listener; }

    private:
        std::queue<Message> m_messageQueue;
        std::mutex m_mutex;
        std::function<void(Message)> m_listener;
    };
}


#endif //GAME_MESSAGEQUEUE_H
