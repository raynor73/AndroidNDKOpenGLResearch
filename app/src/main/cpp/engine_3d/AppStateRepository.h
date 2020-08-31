//
// Created by Igor Lapin on 31/08/2020.
//

#ifndef ENGINE_3D_APP_STATE_REPOSITORY_H
#define ENGINE_3D_APP_STATE_REPOSITORY_H


class AppStateRepository {

public:
    ~AppStateRepository() = default;

    virtual bool isAppInForeground() const = 0;
};


#endif //ENGINE_3D_APP_STATE_REPOSITORY_H
