//
// Created by Igor Lapin on 03/06/2020.
//

#include "GameWrapper.h"
#include "L.h"

void GameWrapper::onDrawFrame() {
    m_messageQueue.update();
    m_scene->Scene::update();
}

void GameWrapper::onSurfaceChanged(int width, int height) {
    if (m_scene == nullptr) {
        m_scene = std::make_shared<ScreenBlinkingScene>(std::make_shared<TimeProvider>());
        L::d("!@£", "Scene created");
    }
}

void GameWrapper::onSurfaceCreated() {
}
