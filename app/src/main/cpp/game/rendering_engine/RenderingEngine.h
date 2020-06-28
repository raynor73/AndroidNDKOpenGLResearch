//
// Created by Igor Lapin on 28/06/2020.
//

#ifndef GAME_RENDERINGENGINE_H
#define GAME_RENDERINGENGINE_H


#include <stack>
#include <game/Scene.h>
#include <main/OpenGLErrorDetector.h>
#include "OpenGLState.h"

class RenderingEngine {

    std::stack<OpenGLState> m_openGLStateStack;
    OpenGLErrorDetector m_openGLErrorDetector;
    std::shared_ptr<DisplayInfo> m_displayInfo;

public:
    RenderingEngine(std::shared_ptr<DisplayInfo> displayInfo) : m_displayInfo(displayInfo) {}
    RenderingEngine(const RenderingEngine&) = delete;
    RenderingEngine(RenderingEngine&&) = delete;

    void render(Scene& scene);

    RenderingEngine& operator=(const RenderingEngine&) = delete;
    RenderingEngine& operator=(RenderingEngine&&) = delete;

private:
    void traverseSceneHierarchy(GameObject& gameObject, const std::function<void(GameObject&)>& callback);

    void pushOpenGLState(const OpenGLState& state);
    void popOpenGLState();
    void applyOpenGLState(const OpenGLState& state);
};


#endif //GAME_RENDERINGENGINE_H
