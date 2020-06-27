//
// Created by Igor Lapin on 03/06/2020.
//

#include "GameWrapper.h"

GameWrapper::GameWrapper(float displayDensityFactor) : m_displayDensityFactor(displayDensityFactor) {
    // do nothing
}

void GameWrapper::onDrawFrame() {
    m_messageQueue.update();
    m_scene->Scene::update();
}

void GameWrapper::onSurfaceChanged(int width, int height) {
    if (m_displayInfo == nullptr) {
        m_displayInfo = std::make_shared<AndroidDisplayInfo>(width, height, m_displayDensityFactor);
    }
    m_displayInfo->setWidth(width);
    m_displayInfo->setHeight(height);

    if (m_scene == nullptr) {
        m_scene = std::make_shared<ScreenBlinkingScene>(
                std::make_shared<TimeProvider>(),
                m_displayInfo
        );
    }
}

void GameWrapper::onSurfaceCreated() {
    // do nothing
}

