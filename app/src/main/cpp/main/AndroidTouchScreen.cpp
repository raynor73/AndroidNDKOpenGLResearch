//
// Created by Igor Lapin on 26/07/2020.
//

#include "GameWrapper.h"
#include "AndroidTouchScreen.h"

AndroidTouchScreen::AndroidTouchScreen(
        std::shared_ptr<MessageQueue::Queue> messageQueue
) : m_messageQueue(messageQueue) {
    m_messageQueue->setListener([&](MessageQueue::Message message) {
        if (message.type == GameWrapper::TOUCH_EVENT_MESSAGE_TYPE_NAME) {
            m_events.push_back(*std::static_pointer_cast<TouchEvent>(message.data));
        }
    });
}

AndroidTouchScreen::~AndroidTouchScreen() {
    m_messageQueue->setListener(nullptr);
}

void AndroidTouchScreen::update() {
    m_events.clear();
}
