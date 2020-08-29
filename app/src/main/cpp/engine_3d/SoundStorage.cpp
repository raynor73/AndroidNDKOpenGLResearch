//
// Created by Igor Lapin on 28/08/2020.
//

#include <exception>
#include <sstream>
#include "SoundStorage.h"

void SoundStorage::putSound(std::string name, std::vector<uint8_t> sound) {
    if (m_storage.count(name) > 0) {
        std::stringstream ss;
        ss << "Sound " << name << " already stored";
        throw std::domain_error(ss.str());
    }

    m_storage.insert({ std::move(name), std::move(sound) });
}

const std::vector<uint8_t>& SoundStorage::getSound(const std::string& name) {
    if (m_storage.count(name) == 0) {
        std::stringstream ss;
        ss << "Sound " << name << " not found";
        throw std::domain_error(ss.str());
    }

    return m_storage.at(name);
}

void SoundStorage::removeSound(const std::string& name) {
    if (m_storage.count(name) == 0) {
        std::stringstream ss;
        ss << "No sound " << name << " to remove";
        throw std::domain_error(ss.str());
    }

    m_storage.erase(name);
}

void SoundStorage::removeAllSounds() {
    m_storage.clear();
}
