//
// Created by Igor Lapin on 31/07/2020.
//

#ifndef ENGINE_3D_UTILS_H
#define ENGINE_3D_UTILS_H


#include <memory>
#include <exception>
#include <string>
#include <functional>
#include <glm/ext/quaternion_float.hpp>
#include <glm/ext.hpp>

namespace Engine3D {

namespace Utils {

    inline glm::quat eulerZXY(float angleZ, float angleX, float angleY) {
        auto rotationMatrix = glm::identity<glm::mat4>();
        rotationMatrix = glm::rotate(rotationMatrix, angleZ, glm::vec3(0, 0, 1));
        rotationMatrix = glm::rotate(rotationMatrix, angleX, glm::vec3(1, 0, 0));
        rotationMatrix = glm::rotate(rotationMatrix, angleY, glm::vec3(0, 1, 0));
        return glm::quat_cast(rotationMatrix);
    }

    template<typename T>
    inline void throwErrorIfNull(const std::shared_ptr<T>& ptr, const std::string& message) {
        if (ptr == nullptr) {
            throw std::domain_error(message);
        }
    }

    template<typename T>
    inline void throwErrorIfNull(const std::shared_ptr<T>& ptr, std::function<std::string(void)> getErrorMessage) {
        if (ptr == nullptr) {
            throw std::domain_error(getErrorMessage());
        }
    }
}

}


#endif //ENGINE_3D_UTILS_H
