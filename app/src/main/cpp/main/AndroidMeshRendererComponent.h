//
// Created by Igor Lapin on 03/07/2020.
//

#ifndef ANDROID_NDK_OPENGL_RESEARCH_ANDROIDMESHRENDERERCOMPONENT_H
#define ANDROID_NDK_OPENGL_RESEARCH_ANDROIDMESHRENDERERCOMPONENT_H


#include <string>
#include <engine_3d/GameObjectComponent.h>

class AndroidMeshRendererComponent : public GameObjectComponent {

public:
    static const std::string TYPE_NAME;

    std::string typeName() const override { return TYPE_NAME; }

    std::shared_ptr<GameObjectComponent> clone() override;
};


#endif //ANDROID_NDK_OPENGL_RESEARCH_ANDROIDMESHRENDERERCOMPONENT_H
