//
// Created by Igor Lapin on 03/07/2020.
//

#ifndef RENDERING_ENGINE_OPENGL_MESH_RENDERER_FACTORY_H
#define RENDERING_ENGINE_OPENGL_MESH_RENDERER_FACTORY_H


#include <game/MeshRendererFactory.h>

class OpenGlMeshRendererFactory : public MeshRendererFactory {

public:
    OpenGlMeshRendererFactory() = default;
    OpenGlMeshRendererFactory(const OpenGlMeshRendererFactory& other) = delete;
    OpenGlMeshRendererFactory(OpenGlMeshRendererFactory&& other) = delete;

    virtual std::shared_ptr<GameObjectComponent> createMeshRenderer(std::vector<std::string> layerNames) override;

    OpenGlMeshRendererFactory& operator=(const OpenGlMeshRendererFactory& other) = delete;
    OpenGlMeshRendererFactory& operator=(OpenGlMeshRendererFactory&& other) = delete;
};


#endif //RENDERING_ENGINE_OPENGL_MESH_RENDERER_FACTORY_H
