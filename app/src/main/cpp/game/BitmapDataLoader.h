//
// Created by Igor Lapin on 01/08/2020.
//

#ifndef GAME_BITMAP_DATA_LOADER_H
#define GAME_BITMAP_DATA_LOADER_H


#include <string>
#include <vector>

struct BitmapInfo {
    std::vector<uint8_t> data;
    uint width;
    uint height;
};

class BitmapDataLoader {

public:
    virtual ~BitmapDataLoader() = default;

    virtual BitmapInfo loadBitmap(const std::string& path) = 0;
};


#endif //GAME_BITMAP_DATA_LOADER_H
