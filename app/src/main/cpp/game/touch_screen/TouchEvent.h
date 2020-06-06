//
// Created by Igor Lapin on 02/06/2020.
//

#ifndef GAME_TOUCHEVENT_H
#define GAME_TOUCHEVENT_H

#include "TouchEventType.h"

struct TouchEvent {
    int id;
    float x, y;
    TouchEventType type;
};

#endif //GAME_TOUCHEVENT_H
