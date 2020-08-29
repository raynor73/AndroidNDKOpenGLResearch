//
// Created by Igor Lapin on 28/08/2020.
//

#include <iffdigest/iffdigest.h>
#include <exception>
#include <sstream>
#include "AndroidSoundLoadingRepository.h"

AndroidSoundLoadingRepository::AndroidSoundLoadingRepository(
        std::shared_ptr<ReadOnlyFsAbstraction> readOnlyFsAbstraction
) : m_readOnlyFsAbstraction(std::move(readOnlyFsAbstraction))
{}

std::vector<uint8_t> AndroidSoundLoadingRepository::loadSound(const std::string& path) {
    auto riffData = m_readOnlyFsAbstraction->readBinaryFileContent(path);

    IFFDigest iff(reinterpret_cast<const char*>(riffData.data()), riffData.size());

    auto i = iff.ck_find(iff_ckid("data"));
    if (i == iff.ck_end()) {
        std::stringstream ss;
        ss << "Data chunk not found in " << path;
        throw std::domain_error(ss.str());
    }
    std::vector<uint8_t> soundData(i->len());
    std::memcpy(soundData.data(), i->dataPtr(), i->len());

    return soundData;
}
