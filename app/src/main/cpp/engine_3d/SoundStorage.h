//
// Created by Igor Lapin on 28/08/2020.
//

#ifndef ENGINE_3D_SOUND_STORAGE_H
#define ENGINE_3D_SOUND_STORAGE_H


#include <unordered_map>
#include <string>
#include <vector>

class SoundStorage {
    
    std::unordered_map<std::string, std::vector<uint8_t>> m_storage;

public:
    SoundStorage() = default;
    SoundStorage(const SoundStorage& other) = delete;
    SoundStorage(SoundStorage&& other) = delete;

    void putSound(std::string name, std::vector<uint8_t> sound);
    const std::vector<uint8_t>& getSound(const std::string& name);
    void removeSound(const std::string& name);
    void removeAllSounds();

    SoundStorage& operator=(const SoundStorage& other) = delete;
    SoundStorage& operator=(SoundStorage&& other) = delete;
};


#endif //ENGINE_3D_SOUND_STORAGE_H
