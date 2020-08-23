//
// Created by Igor Lapin on 28/06/2020.
//

#ifndef ENGINE_3D_CONSTANTS_H
#define ENGINE_3D_CONSTANTS_H


#include <glm/glm.hpp>

namespace Engine3D {

namespace Constants {

    const glm::vec3 DEFAULT_FORWARD_DIRECTION(0, 0, -1);
    const glm::vec3 CAMERA_UP_DIRECTION(0, 1, 0);
    constexpr int MAX_JOINTS = 64;
}

}

#endif //ENGINE_3D_CONSTANTS_H
