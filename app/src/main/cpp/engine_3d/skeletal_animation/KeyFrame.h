//
// Created by Igor Lapin on 17/08/2020.
//

#ifndef ENGINE_3D_KEYFRAME_H
#define ENGINE_3D_KEYFRAME_H


#include <string>
#include <unordered_map>
#include <glm/mat4x4.hpp>

struct KeyFrame {
    float time;
    std::unordered_map<std::string, glm::mat4> jointLocalTransform;
};


#endif //ENGINE_3D_KEYFRAME_H
