//
// Created by Igor Lapin on 06/06/2020.
//

#ifndef GAME_SCREENBLINKINGSCENE_H
#define GAME_SCREENBLINKINGSCENE_H


#include <memory>
#include "../Scene.h"

class ScreenBlinkingScene : public Scene {

    float m_elapsed;
    bool m_isWhite;

public:
    explicit ScreenBlinkingScene(
            std::shared_ptr<TimeProvider> timeProvider,
            std::shared_ptr<DisplayInfo> displayInfo
    );

protected:
    virtual void update(float dt) override;
};


#endif //GAME_SCREENBLINKINGSCENE_H
