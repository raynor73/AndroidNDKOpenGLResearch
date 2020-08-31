//
// Created by Igor Lapin on 03/06/2020.
//

#ifndef GAME_DISPLAYINFO_H
#define GAME_DISPLAYINFO_H


class DisplayInfo {

public:
    virtual float width() const = 0;
    virtual float height() const = 0;
    virtual float densityFactor() const = 0;
};


#endif //GAME_DISPLAYINFO_H
