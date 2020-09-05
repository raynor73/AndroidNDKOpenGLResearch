//
// Created by Igor Lapin on 05/09/2020.
//

#ifndef ENGINE_3D_INTERPOLATOR_H
#define ENGINE_3D_INTERPOLATOR_H


class Interpolator {

public:
    virtual ~Interpolator() = default;

    virtual float interpolate(float input) = 0;
};


#endif //ENGINE_3D_INTERPOLATOR_H
