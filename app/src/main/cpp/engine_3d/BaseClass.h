//
// Created by Igor Lapin on 23/11/2020.
//

#ifndef GAME_BASE_CLASS_H
#define GAME_BASE_CLASS_H


class BaseClass {

public:
    BaseClass() = default;
    BaseClass(const BaseClass&) = delete;
    BaseClass(BaseClass&&) = delete;

    virtual ~BaseClass() = default;

    BaseClass& operator=(const BaseClass&) = delete;
    BaseClass& operator=(BaseClass&&) = delete;
};


#endif //GAME_BASE_CLASS_H
