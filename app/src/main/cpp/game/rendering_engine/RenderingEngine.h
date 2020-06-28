//
// Created by Igor Lapin on 28/06/2020.
//

#ifndef GAME_RENDERINGENGINE_H
#define GAME_RENDERINGENGINE_H


#include <game/Scene.h>

class RenderingEngine {

public:
    RenderingEngine() = default;
    RenderingEngine(const RenderingEngine&) = delete;
    RenderingEngine(RenderingEngine&&) = delete;

    void render(Scene& scene);

    RenderingEngine& operator=(const RenderingEngine&) = delete;
    RenderingEngine& operator=(RenderingEngine&&) = delete;
};


#endif //GAME_RENDERINGENGINE_H
