//
// Created by Igor Lapin on 09/07/2020.
//

#ifndef ENGINE_3D_MATERIAL_H
#define ENGINE_3D_MATERIAL_H


#include <glm/glm.hpp>

struct Material {
    glm::vec4 diffuseColor;
    std::string textureName;
    bool useDiffuseColor;
    bool isTranslucent;
    bool isWireframe;
    bool isUnlit;
};


#endif //ENGINE_3D_MATERIAL_H
