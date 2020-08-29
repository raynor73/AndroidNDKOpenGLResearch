//
// Created by Igor Lapin on 28/08/2020.
//

#ifndef ENGINE_3D_SOUND_LOADING_REPOSITORY_H
#define ENGINE_3D_SOUND_LOADING_REPOSITORY_H


#include <vector>
#include <string>

class SoundLoadingRepository {

public:
    virtual ~SoundLoadingRepository() = default;

    virtual std::vector<uint8_t> loadSound(const std::string& path) = 0;
};


#endif //ENGINE_3D_SOUND_LOADING_REPOSITORY_H
