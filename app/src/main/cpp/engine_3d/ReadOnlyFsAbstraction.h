//
// Created by Igor Lapin on 26/08/2020.
//

#ifndef ENGINE_3D_READ_ONLY_FS_ABSTRACTION_H
#define ENGINE_3D_READ_ONLY_FS_ABSTRACTION_H


#include <string>

class ReadOnlyFsAbstraction {

public:
    virtual ~ReadOnlyFsAbstraction() = default;

    virtual bool isFileExists(const std::string& path) = 0;

    virtual std::vector<uint8_t> readBinaryFileContent(const std::string& path) = 0;
    virtual std::string readTextFileContent(const std::string& path) = 0;
};


#endif //ENGINE_3D_READ_ONLY_FS_ABSTRACTION_H
