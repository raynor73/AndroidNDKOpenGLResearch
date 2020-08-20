//
// Created by Igor Lapin on 19/08/2020.
//

#ifndef ENGINE_3D_SKELETAL_ANIMATION_H
#define ENGINE_3D_SKELETAL_ANIMATION_H


#include <vector>
#include "KeyFrame.h"

struct SkeletalAnimation {
    float length;
    std::vector<KeyFrame> keyFrames;
};


#endif //ENGINE_3D_SKELETAL_ANIMATION_H
