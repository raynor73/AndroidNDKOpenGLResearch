//
// Created by Igor Lapin on 19/08/2020.
//

#ifndef ENGINE_3D_SKELETAL_ANIMATION_H
#define ENGINE_3D_SKELETAL_ANIMATION_H


#include <vector>
#include <memory>
#include "KeyFrame.h"
#include "Joint.h"

struct SkeletalAnimation {
    float length;
    std::shared_ptr<Joint> rootJoint;
    std::vector<std::shared_ptr<Joint>> joints;
    std::vector<KeyFrame> keyFrames;
};


#endif //ENGINE_3D_SKELETAL_ANIMATION_H
