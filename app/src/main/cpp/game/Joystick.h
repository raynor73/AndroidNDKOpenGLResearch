//
// Created by Igor Lapin on 09/08/2020.
//

#ifndef GAME_JOYSTICK_H
#define GAME_JOYSTICK_H


struct JoystickPosition {
    float x;
    float y;
};

class Joystick {

public:
    virtual const JoystickPosition& position() = 0;
    virtual ~Joystick() = default;
};


#endif //GAME_JOYSTICK_H
