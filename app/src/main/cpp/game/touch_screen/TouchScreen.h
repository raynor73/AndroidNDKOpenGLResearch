//
// Created by Igor Lapin on 02/06/2020.
//

#ifndef GAME_TOUCHSCREEN_H
#define GAME_TOUCHSCREEN_H


#include <memory>
#include "TouchEvent.h"

class TouchScreen {

public:
    virtual std::vector<TouchEvent> events() const = 0;
};


#endif //GAME_TOUCHSCREEN_H
