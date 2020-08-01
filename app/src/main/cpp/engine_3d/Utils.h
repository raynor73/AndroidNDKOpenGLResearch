//
// Created by Igor Lapin on 31/07/2020.
//

#ifndef ENGINE_3D_UTILS_H
#define ENGINE_3D_UTILS_H


#include <memory>
#include <exception>
#include <string>

namespace Engine3D {

namespace Utils {
    template<typename T>
    inline void throwErrorIfNull(const std::shared_ptr<T>& ptr, const std::string& message) {
        if (ptr == nullptr) {
            throw std::domain_error(message);
        }
    }
}

}


#endif //ENGINE_3D_UTILS_H
