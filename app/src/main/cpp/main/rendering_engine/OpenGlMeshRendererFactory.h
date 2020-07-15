//
// Created by Igor Lapin on 03/07/2020.
//

#ifndef RENDERING_ENGINE_OPENGL_MESH_RENDERER_FACTORY_H
#define RENDERING_ENGINE_OPENGL_MESH_RENDERER_FACTORY_H


#include <game/MeshRendererFactory.h>

class OpenGlMeshRendererFactory : public MeshRendererFactory {

    std::shared_ptr<OpenGLErrorDetector> m_openGlErrorDetector;
    std::shared_ptr<OpenGLGeometryBuffersStorage> m_geometryBuffersStorage;

public:
    OpenGlMeshRendererFactory(
            std::shared_ptr<OpenGLGeometryBuffersStorage> geometryBuffersStorage,
            std::shared_ptr<OpenGLErrorDetector> openGlErrorDetector
    ) : m_geometryBuffersStorage(geometryBuffersStorage),
        m_openGlErrorDetector(openGlErrorDetector)
    {}
    OpenGlMeshRendererFactory(const OpenGlMeshRendererFactory& other) = delete;
    OpenGlMeshRendererFactory(OpenGlMeshRendererFactory&& other) = delete;

    virtual std::shared_ptr<GameObjectComponent> createMeshRenderer(std::vector<std::string> layerNames) override;

    OpenGlMeshRendererFactory& operator=(const OpenGlMeshRendererFactory& other) = delete;
    OpenGlMeshRendererFactory& operator=(OpenGlMeshRendererFactory&& other) = delete;
};


#endif //RENDERING_ENGINE_OPENGL_MESH_RENDERER_FACTORY_H
