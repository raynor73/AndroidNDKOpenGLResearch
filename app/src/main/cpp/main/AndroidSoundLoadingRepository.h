//
// Created by Igor Lapin on 28/08/2020.
//

#ifndef ANDROID_NDK_OPENGL_RESEARCH_ANDROIDSOUNDLOADINGREPOSITORY_H
#define ANDROID_NDK_OPENGL_RESEARCH_ANDROIDSOUNDLOADINGREPOSITORY_H


#include <memory>
#include <utility>
#include <engine_3d/ReadOnlyFsAbstraction.h>
#include <engine_3d/SoundLoadingRepository.h>

class AndroidSoundLoadingRepository : public SoundLoadingRepository {

    std::shared_ptr<ReadOnlyFsAbstraction> m_readOnlyFsAbstraction;

public:
    AndroidSoundLoadingRepository(std::shared_ptr<ReadOnlyFsAbstraction> readOnlyFsAbstraction);
    AndroidSoundLoadingRepository(const AndroidSoundLoadingRepository&) = delete;
    AndroidSoundLoadingRepository(AndroidSoundLoadingRepository&&) = delete;

    virtual std::vector<uint8_t> loadSound(const std::string& path) override;

    AndroidSoundLoadingRepository& operator= (const AndroidSoundLoadingRepository&) = delete;
    AndroidSoundLoadingRepository& operator= (AndroidSoundLoadingRepository&&) = delete;
};


#endif //ANDROID_NDK_OPENGL_RESEARCH_ANDROIDSOUNDLOADINGREPOSITORY_H
