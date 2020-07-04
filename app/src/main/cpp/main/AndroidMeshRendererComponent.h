//
// Created by Igor Lapin on 03/07/2020.
//

#ifndef ANDROID_NDK_OPENGL_RESEARCH_ANDROIDMESHRENDERERCOMPONENT_H
#define ANDROID_NDK_OPENGL_RESEARCH_ANDROIDMESHRENDERERCOMPONENT_H


#include <string>
#include <vector>
#include <utility>
#include <engine_3d/GameObjectComponent.h>

class AndroidMeshRendererComponent : public GameObjectComponent {

    std::vector<std::string> m_layerNames;

public:
    static const std::string TYPE_NAME;

    AndroidMeshRendererComponent(std::vector<std::string> layerNames) : m_layerNames(std::move(layerNames)) {}

    const std::vector<std::string>& layerNames() const { return m_layerNames; }
    void setLayerNames(std::vector<std::string> layerNames) { m_layerNames = std::move(layerNames); }

    virtual const std::string& typeName() const override { return TYPE_NAME; }

    virtual std::shared_ptr<GameObjectComponent> clone() override;
};


#endif //ANDROID_NDK_OPENGL_RESEARCH_ANDROIDMESHRENDERERCOMPONENT_H
