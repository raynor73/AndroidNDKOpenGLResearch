//
// Created by Igor Lapin on 04/06/2020.
//

#ifndef ANDROID_NDK_OPENGL_RESEARCH_L_H
#define ANDROID_NDK_OPENGL_RESEARCH_L_H


#include <android/log.h>
#include <string>

class L {

public:

    static void d(const std::string &tag, const std::string &msg) {
        __android_log_print(ANDROID_LOG_DEBUG, tag.c_str(), "%s", msg.c_str());
    }
};


#endif //ANDROID_NDK_OPENGL_RESEARCH_L_H
