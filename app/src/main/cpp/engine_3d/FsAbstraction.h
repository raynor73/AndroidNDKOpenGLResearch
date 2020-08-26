//
// Created by Igor Lapin on 24/08/2020.
//

#ifndef ENGINE_3D_FS_ABSTRACTION_H
#define ENGINE_3D_FS_ABSTRACTION_H


#include <vector>
#include <string>

class FsAbstraction {

public:
    virtual ~FsAbstraction() = default;

    virtual bool isFileExists(const std::string& path) = 0;

    virtual std::vector<uint8_t> readBinaryFileContent(const std::string& path) = 0;
    virtual void writeBinaryFileContent(const std::string& path, const std::vector<uint8_t>& data) = 0;

    virtual std::string readTextFileContent(const std::string& path) = 0;
    virtual void writeTextFileContent(const std::string& path, const std::string& text) = 0;

    virtual void deleteFile(const std::string& path) = 0;
};


#endif //ENGINE_3D_FS_ABSTRACTION_H
