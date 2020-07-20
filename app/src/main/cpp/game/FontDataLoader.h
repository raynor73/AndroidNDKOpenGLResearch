//
// Created by Igor Lapin on 20/07/2020.
//

#ifndef GAME_FONT_DATA_LOADER_H
#define GAME_FONT_DATA_LOADER_H


#include <string>
#include <vector>

class FontDataLoader {

public:
    virtual std::vector<uint8_t> loadFontData(const std::string& fontPath) = 0;
};


#endif //GAME_FONT_DATA_LOADER_H
