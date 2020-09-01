//
// Created by Igor Lapin on 03/07/2020.
//

#include <GLES2/gl2.h>
#include <memory>
#include <sstream>
#include <exception>
#include <glm/gtx/transform.hpp>
#include <glm/mat4x4.hpp>
#include <engine_3d/GameObject.h>
#include <engine_3d/MeshComponent.h>
#include <engine_3d/MaterialComponent.h>
#include <engine_3d/skeletal_animation/SkeletalAnimationPlayerComponent.h>
#include <engine_3d/Constants.h>
#include "OpenGlMeshRendererComponent.h"

const std::string OpenGlMeshRendererComponent::TYPE_NAME = "AndroidMeshRendererComponent";

void OpenGlMeshRendererComponent::render(
        const OpenGlShaderProgramContainer& shaderProgramContainer,
        const glm::mat4x4& modelMatrix,
        const glm::mat4x4& viewMatrix,
        const glm::mat4x4& projectionMatrix,
        ShaderType shaderType
) {
    throwErrorIfNoGameObject();

    if (!m_isEnabled || !m_gameObject->isEnabled()) {
        return;
    }

    auto meshComponent = std::static_pointer_cast<MeshComponent>(m_gameObject->findComponent(MeshComponent::TYPE_NAME));
    if (meshComponent == nullptr) {
        std::stringstream ss;
        ss << "No mesh to render for game object: " << m_gameObject->name();
        throw std::domain_error(ss.str());
    }

    auto vboOptional = m_geometryBuffersStorage->findVbo(meshComponent->meshName());
    if (!vboOptional) {
        std::stringstream ss;
        ss << "VBO " << meshComponent->meshName() << " not found";
        throw std::domain_error(ss.str());
    }
    auto vbo = vboOptional.value();

    auto iboInfoOptional = m_geometryBuffersStorage->findIbo(meshComponent->meshName());
    if (!iboInfoOptional) {
        std::stringstream ss;
        ss << "IBO " << meshComponent->meshName() << " not found";
        throw std::domain_error(ss.str());
    }
    auto iboInfo = iboInfoOptional.value();

    auto materialComponent = std::static_pointer_cast<MaterialComponent>(
            m_gameObject->findComponent(MaterialComponent::TYPE_NAME)
    );
    auto material = materialComponent->material();

    if (
            (material.isUnlit && shaderType != ShaderType::UNLIT) ||
            (!material.isUnlit && shaderType == ShaderType::UNLIT)
    ) {
        return;
    }

    if (meshComponent == nullptr) {
        std::stringstream ss;
        ss << "No material for rendering game object: " << m_gameObject->name();
        throw std::domain_error(ss.str());
    }

    /*if (isShadowMapRendering && !material.castShadows) {
        return
    }*/

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboInfo.ibo);

    auto vertexPositionAttribute = shaderProgramContainer.positionAttribute();
    if (vertexPositionAttribute >= 0) {
        glVertexAttribPointer(
                vertexPositionAttribute,
                Vertex::VERTEX_POSITION_COMPONENTS,
                GL_FLOAT,
                false,
                Vertex::VERTEX_COMPONENTS * sizeof(float),
                reinterpret_cast<void*>(0)
        );
        glEnableVertexAttribArray(vertexPositionAttribute);
    }

    auto vertexNormalAttribute = shaderProgramContainer.normalAttribute();
    if (vertexNormalAttribute >= 0) {
        glVertexAttribPointer(
                vertexNormalAttribute,
                Vertex::VERTEX_NORMAL_COMPONENTS,
                GL_FLOAT,
                false,
                Vertex::VERTEX_COMPONENTS * sizeof(float),
                reinterpret_cast<void*>(Vertex::VERTEX_POSITION_COMPONENTS * sizeof(float))
        );
        glEnableVertexAttribArray(vertexNormalAttribute);
    }

    auto vertexUvAttribute = shaderProgramContainer.uvAttribute();
    if (vertexUvAttribute >= 0) {
        glVertexAttribPointer(
                vertexUvAttribute,
                Vertex::VERTEX_UV_COMPONENTS,
                GL_FLOAT,
                false,
                Vertex::VERTEX_COMPONENTS * sizeof(float),
                reinterpret_cast<void*>(
                        (Vertex::VERTEX_POSITION_COMPONENTS + Vertex::VERTEX_NORMAL_COMPONENTS) * sizeof(float)
                )
        );
        glEnableVertexAttribArray(vertexUvAttribute);
    }

    auto vertexJointIndicesAttribute = shaderProgramContainer.jointIndicesAttribute();
    if (vertexJointIndicesAttribute >= 0) {
        glVertexAttribPointer(
                vertexJointIndicesAttribute,
                Vertex::VERTEX_JOINT_INDICES_COMPONENTS,
                GL_FLOAT,
                false,
                Vertex::VERTEX_COMPONENTS * sizeof(float),
                reinterpret_cast<void*>((
                        Vertex::VERTEX_POSITION_COMPONENTS +
                        Vertex::VERTEX_NORMAL_COMPONENTS +
                        Vertex::VERTEX_UV_COMPONENTS
                ) * sizeof(float))
        );
        glEnableVertexAttribArray(vertexJointIndicesAttribute);
    }

    auto vertexJointWeightsAttribute = shaderProgramContainer.jointWeightsAttribute();
    if (vertexJointWeightsAttribute >= 0) {
        glVertexAttribPointer(
                vertexJointWeightsAttribute,
                Vertex::VERTEX_JOINT_WEIGHTS_COMPONENTS,
                GL_FLOAT,
                false,
                Vertex::VERTEX_COMPONENTS * sizeof(float),
                reinterpret_cast<void*>((
                        Vertex::VERTEX_POSITION_COMPONENTS +
                        Vertex::VERTEX_NORMAL_COMPONENTS +
                        Vertex::VERTEX_UV_COMPONENTS +
                        Vertex::VERTEX_JOINT_INDICES_COMPONENTS
                ) * sizeof(float))
        );
        glEnableVertexAttribArray(vertexJointWeightsAttribute);
    }

    if (auto mvpMatrixUniform = shaderProgramContainer.mvpMatrixUniform(); mvpMatrixUniform >= 0) {
        glm::mat4x4 mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;
        glUniformMatrix4fv(mvpMatrixUniform, 1, false, &mvpMatrix[0][0]);
    }

    if (auto modelMatrixUniform = shaderProgramContainer.modelMatrixUniform(); modelMatrixUniform >=0) {
        glUniformMatrix4fv(modelMatrixUniform, 1, false, &modelMatrix[0][0]);
    }

    /*shaderProgram.lightMvpMatrixUniform.takeIf { it >= 0 }?.let { lightMvpMatrixUniform ->
            tmpMatrix.set(lightProjectionMatrix)
            tmpMatrix.mul(lightViewMatrix)
            tmpMatrix.mul(lightModelMatrix)
            tmpMatrix.get(tmpFloatArray)
            GLES20.glUniformMatrix4fv(lightMvpMatrixUniform, 1, false, tmpFloatArray, 0)
    }

    shaderProgram.biasMatrixUniform.takeIf { it >= 0 }?.let { biasMatrixUniform ->
            BIAS_MATRIX.get(tmpFloatArray)
            GLES20.glUniformMatrix4fv(biasMatrixUniform, 1, false, tmpFloatArray, 0)
    }*/

    if (auto diffuseColorUniform = shaderProgramContainer.diffuseColorUniform(); diffuseColorUniform >= 0) {
        glUniform4f(
                diffuseColorUniform,
                material.diffuseColor.x,
                material.diffuseColor.y,
                material.diffuseColor.z,
                material.diffuseColor.w
        );
    }

    if (auto useDiffuseColorUniform = shaderProgramContainer.useDiffuseColorUniform(); useDiffuseColorUniform >= 0) {
        glUniform1i(useDiffuseColorUniform, material.useDiffuseColor ? GL_TRUE : GL_FALSE);
    }

    if (!material.useDiffuseColor) {
        if (auto textureUniform = shaderProgramContainer.textureUniform(); textureUniform >= 0) {
            auto textureInfo = m_texturesRepository->findTexture(material.textureName);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, textureInfo->texture);
            glUniform1i(textureUniform, 0);
        }
    }
    /*shaderProgram.receiveShadows.glUniform1i(material.receiveShadows.toGLBoolean())
    safeLet(shadowMapTextureInfo, shaderProgram.shadowMapUniform.takeIf { it >= 0 }) { textureInfo, shadowMapUniform ->
                GLES20.glActiveTexture(GLES20.GL_TEXTURE1)
        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, textureInfo.texture)
        GLES20.glUniform1i(shadowMapUniform, 1)
    }*/

    auto skeletalAnimationPlayer = m_gameObject->findComponent<SkeletalAnimationPlayerComponent>();
    auto hasSkeletalAnimation = false;
    if (skeletalAnimationPlayer != nullptr && !skeletalAnimationPlayer->jointTransforms().empty()) {
        if (
                auto jointTransformsUniform = shaderProgramContainer.jointTransformsUniform();
                jointTransformsUniform >= 0
        ) {
            hasSkeletalAnimation = true;

            glUniformMatrix4fv(
                    jointTransformsUniform,
                    Engine3D::Constants::MAX_JOINTS,
                    false,
                    &skeletalAnimationPlayer->jointTransforms()[0][0][0]
            );
        }
    }
    if (
            auto hasSkeletalAnimationUniform = shaderProgramContainer.hasSkeletalAnimationUniform();
            hasSkeletalAnimationUniform >= 0
    ) {
        glUniform1i(hasSkeletalAnimationUniform, hasSkeletalAnimation ? GL_TRUE : GL_FALSE);
    }

    if (material.isDoubleSided) {
        glDisable(GL_CULL_FACE);
    } else {
        glEnable(GL_CULL_FACE);
    }

    GLenum mode;
    if (material.isWireframe) {
        mode = GL_LINE_STRIP;
    } else {
        mode = GL_TRIANGLES;
    }

    if (material.isTranslucent) {
        glCullFace(GL_FRONT);
        glDrawElements(
                mode,
                iboInfo.numberOfIndices,
                GL_UNSIGNED_SHORT,
                reinterpret_cast<void*>(0)
        );

        glCullFace(GL_BACK);
        glDrawElements(
                mode,
                iboInfo.numberOfIndices,
                GL_UNSIGNED_SHORT,
                reinterpret_cast<void*>(0)
        );
    } else {
        glCullFace(GL_BACK);
        glDrawElements(
                mode,
                iboInfo.numberOfIndices,
                GL_UNSIGNED_SHORT,
                reinterpret_cast<void*>(0)
        );
    }

    if (vertexPositionAttribute >= 0) {
        glDisableVertexAttribArray(vertexPositionAttribute);
    }
    if (vertexNormalAttribute >= 0) {
        glDisableVertexAttribArray(vertexNormalAttribute);
    }
    if (vertexUvAttribute >= 0) {
        glDisableVertexAttribArray(vertexUvAttribute);
    }
    if (vertexJointIndicesAttribute >= 0) {
        glDisableVertexAttribArray(vertexJointIndicesAttribute);
    }
    if (vertexJointWeightsAttribute >= 0) {
        glDisableVertexAttribArray(vertexJointWeightsAttribute);
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    m_openGlErrorDetector->checkOpenGLErrors("OpenGlMeshRendererComponent::render");
}

std::shared_ptr<GameObjectComponent> OpenGlMeshRendererComponent::clone() {
    auto clone = std::make_shared<OpenGlMeshRendererComponent>(
            m_layerNames,
            m_geometryBuffersStorage,
            m_texturesRepository,
            m_openGlErrorDetector
    );
    clone->setEnabled(m_isEnabled);
    return clone;
}
