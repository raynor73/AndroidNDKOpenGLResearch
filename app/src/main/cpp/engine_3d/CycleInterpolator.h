//
// Created by Igor Lapin on 05/09/2020.
//

#ifndef ENGINE_3D_CYCLE_INTERPOLATOR_H
#define ENGINE_3D_CYCLE_INTERPOLATOR_H


#include "Interpolator.h"

class CycleInterpolator : public Interpolator {

    float m_period;

public:
    CycleInterpolator(float period);

    virtual float interpolate(float input) override;
};


#endif //ENGINE_3D_CYCLE_INTERPOLATOR_H
