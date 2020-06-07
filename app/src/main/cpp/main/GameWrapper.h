//
// Created by Igor Lapin on 03/06/2020.
//

#ifndef ANDROID_NDK_OPENGL_RESEARCH_GAMEWRAPPER_H
#define ANDROID_NDK_OPENGL_RESEARCH_GAMEWRAPPER_H


#include <memory>
#include "../game/dev_scenes/ScreenBlinkingScene.h"
#include "../game/MessageQueue.h"
#include "AndroidDisplayInfo.h"

class GameWrapper {

public:
    explicit GameWrapper(float displayDensityFactor);

    void onDrawFrame();
    void onSurfaceChanged(int width, int height);
    void onSurfaceCreated();

private:
    MessageQueue m_messageQueue;
    float m_displayDensityFactor;
    std::shared_ptr<AndroidDisplayInfo> m_displayInfo;
    std::shared_ptr<ScreenBlinkingScene> m_scene;
};


#endif //ANDROID_NDK_OPENGL_RESEARCH_GAMEWRAPPER_H
