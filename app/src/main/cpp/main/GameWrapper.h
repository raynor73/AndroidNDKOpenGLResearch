//
// Created by Igor Lapin on 03/06/2020.
//

#ifndef ANDROID_NDK_OPENGL_RESEARCH_GAMEWRAPPER_H
#define ANDROID_NDK_OPENGL_RESEARCH_GAMEWRAPPER_H


#include <memory>
#include <game/MessageQueue.h>
#include <game/rendering_engine/RenderingEngine.h>
#include "AndroidDisplayInfo.h"

class GameWrapper {

    MessageQueue m_messageQueue;
    float m_displayDensityFactor;
    std::shared_ptr<AndroidDisplayInfo> m_displayInfo;
    std::shared_ptr<Scene> m_scene;
    std::shared_ptr<RenderingEngine> m_renderingEngine;

public:
    explicit GameWrapper(float displayDensityFactor);

    void onDrawFrame();
    void onSurfaceChanged(int width, int height);
    void onSurfaceCreated();
};


#endif //ANDROID_NDK_OPENGL_RESEARCH_GAMEWRAPPER_H
