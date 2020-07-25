//
// Created by Igor Lapin on 25/07/2020.
//

#ifndef GAME_FPS_CALCULATOR_H
#define GAME_FPS_CALCULATOR_H


#include <list>

class FpsCalculator {

    float m_fps;
    float m_internalFps;
    float m_elapsedTime;
    std::list<float> m_buffer;

public:
    float fps() const { return m_fps; }

    void update(float dt);

private:
    const int MAX_BUFFER_SIZE = 100;
    const float REFRESH_PERIOD_SECONDS = 1;
};


#endif //GAME_FPS_CALCULATOR_H
