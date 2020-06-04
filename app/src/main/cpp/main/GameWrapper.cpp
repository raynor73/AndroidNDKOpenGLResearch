//
// Created by Igor Lapin on 03/06/2020.
//

#include <sstream>
#include "GameWrapper.h"
#include "L.h"

void GameWrapper::onDrawFrame() {

}

void GameWrapper::onSurfaceChanged(int width, int height) {
    std::stringstream ss;
    ss << "onSurfaceChanged: " << width << "x" << height;
    L::d("!@£", ss.str());
}

void GameWrapper::onSurfaceCreated() {
    L::d("!@£", "onSurfaceCreated");
}
