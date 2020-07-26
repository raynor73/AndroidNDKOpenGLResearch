//
// Created by Igor Lapin on 26/07/2020.
//

#ifndef ANDROID_NDK_OPENGL_RESEARCH_ANDROIDTOUCHSCREEN_H
#define ANDROID_NDK_OPENGL_RESEARCH_ANDROIDTOUCHSCREEN_H


#include <memory>
#include <vector>
#include <game/MessageQueue.h>
#include <game/touch_screen/TouchScreen.h>

class AndroidTouchScreen : public TouchScreen {

    std::shared_ptr<MessageQueue::Queue> m_messageQueue;
    std::vector<TouchEvent> m_events;

public:
    AndroidTouchScreen(std::shared_ptr<MessageQueue::Queue> messageQueue);
    AndroidTouchScreen(const AndroidTouchScreen&) = delete;
    AndroidTouchScreen(AndroidTouchScreen&&) = delete;

    virtual ~AndroidTouchScreen() override;

    virtual const std::vector<TouchEvent>& events() const override { return m_events; }

    void update();

    AndroidTouchScreen& operator=(const AndroidTouchScreen&) = delete;
    AndroidTouchScreen& operator=(AndroidTouchScreen&&) = delete;
};


#endif //ANDROID_NDK_OPENGL_RESEARCH_ANDROIDTOUCHSCREEN_H
