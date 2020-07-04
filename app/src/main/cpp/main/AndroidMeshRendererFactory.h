//
// Created by Igor Lapin on 03/07/2020.
//

#ifndef ANDROID_NDK_OPENGL_RESEARCH_ANDROIDMESHRENDERERFACTORY_H
#define ANDROID_NDK_OPENGL_RESEARCH_ANDROIDMESHRENDERERFACTORY_H


#include <game/MeshRendererFactory.h>

class AndroidMeshRendererFactory : public MeshRendererFactory {

public:
    AndroidMeshRendererFactory() = default;
    AndroidMeshRendererFactory(const AndroidMeshRendererFactory& other) = delete;
    AndroidMeshRendererFactory(AndroidMeshRendererFactory&& other) = delete;

    virtual std::shared_ptr<GameObjectComponent> createMeshRenderer(std::vector<std::string> layerNames) override;

    AndroidMeshRendererFactory& operator=(const AndroidMeshRendererFactory& other) = delete;
    AndroidMeshRendererFactory& operator=(AndroidMeshRendererFactory&& other) = delete;
};


#endif //ANDROID_NDK_OPENGL_RESEARCH_ANDROIDMESHRENDERERFACTORY_H
