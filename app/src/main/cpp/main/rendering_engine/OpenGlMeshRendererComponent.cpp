//
// Created by Igor Lapin on 03/07/2020.
//

#include <GLES2/gl2.h>
#include <memory>
#include <sstream>
#include <exception>
#include <glm/gtx/compatibility.hpp>
#include <glm/mat4x4.hpp>
#include <engine_3d/GameObject.h>
#include <engine_3d/MeshComponent.h>
#include <engine_3d/MaterialComponent.h>
#include "OpenGlMeshRendererComponent.h"

const std::string OpenGlMeshRendererComponent::TYPE_NAME = "AndroidMeshRendererComponent";

static GLuint vbo2;
static GLuint ibo2;
static int numberOfIndices2 = -1;


void OpenGlMeshRendererComponent::render(
        const OpenGlShaderProgramContainer& shaderProgramContainer,
        const glm::mat4x4& modelMatrix,
        const glm::mat4x4& viewMatrix,
        const glm::mat4x4& projectionMatrix
) {
    if (!m_isEnabled || m_gameObject == nullptr || !m_gameObject->isEnabled()) {
        return;
    }


    {
        if (numberOfIndices2 < 0) {
            float vertexData[] = {
                    -5, -5, 0,
                    -5,  5, 0,
                    5,  5, 0,
                    5, -5, 0
            };
            uint16_t indices[] = {
                    0, 2, 1, 0, 3, 2
            };
            numberOfIndices2 = sizeof(indices) / sizeof(uint16_t);

            glGenBuffers(1, &vbo2);
            glBindBuffer(GL_ARRAY_BUFFER, vbo2);
            glBufferData(
                    GL_ARRAY_BUFFER,
                    sizeof(vertexData),
                    vertexData,
                    GL_STATIC_DRAW
            );
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            glGenBuffers(1, &ibo2);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo2);
            glBufferData(
                    GL_ELEMENT_ARRAY_BUFFER,
                    sizeof(indices),
                    indices,
                    GL_STATIC_DRAW
            );
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }

        glFrontFace(GL_CCW);
        glCullFace(GL_BACK);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glEnable(GL_SCISSOR_TEST);

        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        /*auto shaderContainer = m_shadersRepository->getShaderProgramContainer("unlit");
        glUseProgram(shaderContainer.shaderProgram());*/

        glBindBuffer(GL_ARRAY_BUFFER, vbo2);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo2);

        auto vertexPositionAttribute = shaderProgramContainer.positionAttribute();
        glVertexAttribPointer(
                vertexPositionAttribute,
                Vertex::VERTEX_POSITION_COMPONENTS,
                GL_FLOAT,
                false,
                Vertex::VERTEX_POSITION_COMPONENTS * sizeof(float),
                reinterpret_cast<void*>(0)
        );
        glEnableVertexAttribArray(vertexPositionAttribute);

        auto projectionMatrix2 = glm::ortho(0.0f, 1280.0f, 0.0f, 800.0f, 0.1f, 100.0f);
        auto viewMatrix2 = glm::lookAt(glm::vec3(0, 0, 1), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
        auto modelMatrix2 = glm::identity<glm::mat4x4>();//glm::scale(glm::identity<glm::mat4x4>(), glm::vec3(300.0f, 300.0f, 1.0f));
        glm::mat4x4 mvpMatrix = projectionMatrix2 * viewMatrix2 * modelMatrix2;
        glUniformMatrix4fv(shaderProgramContainer.mvpMatrixUniform(), 1, false, &mvpMatrix[0][0]);

        glUniform4f(
                shaderProgramContainer.diffuseColorUniform(),
                1,
                1,
                1,
                1
        );
        glUniform1i(shaderProgramContainer.useDiffuseColorUniform(), GL_TRUE);

        glUniform1i(shaderProgramContainer.hasSkeletalAnimationUniform(), GL_FALSE);

        glDrawElements(
                GL_TRIANGLES,
                numberOfIndices2,
                GL_UNSIGNED_SHORT,
                reinterpret_cast<void*>(0)
        );

        glDisableVertexAttribArray(vertexPositionAttribute);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        m_openGlErrorDetector->checkOpenGLErrors("RenderingEngine::render");

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
    if (meshComponent == nullptr) {
        std::stringstream ss;
        ss << "No material for rendering game object: " << m_gameObject->name();
        throw std::domain_error(ss.str());
    }

    /*if (isShadowMapRendering && !material.castShadows) {
        return
    }

    if (
            (material.isUnlit && shaderProgram !is ShaderProgramInfo.UnlitShaderProgram) or
    (!material.isUnlit && shaderProgram is ShaderProgramInfo.UnlitShaderProgram)
    ) {
        return
    }

    if (
            (material.isTranslucent && !isTranslucentRendering) or
            (!material.isTranslucent && isTranslucentRendering)
            ) {
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

    /*shaderProgram.jointIndicesAttribute.takeIf { it >= 0 }?.let { jointIndicesAttribute ->
            GLES20.glVertexAttribPointer(
            jointIndicesAttribute,
            NUMBER_OF_JOINT_INDICES,
            GLES20.GL_FLOAT,
            false,
            VERTEX_COMPONENTS * BYTES_IN_FLOAT,
            (VERTEX_COORDINATE_COMPONENTS +
             NORMAL_COMPONENTS +
             TEXTURE_COORDINATE_COMPONENTS) * BYTES_IN_FLOAT
    )
            GLES20.glEnableVertexAttribArray(jointIndicesAttribute)
    }

    shaderProgram.jointWeightsAttribute.takeIf { it >= 0 }?.let { jointWeightsAttribute ->
            GLES20.glVertexAttribPointer(
            jointWeightsAttribute,
            NUMBER_OF_JOINT_WEIGHTS,
            GLES20.GL_FLOAT,
            false,
            VERTEX_COMPONENTS * BYTES_IN_FLOAT,
            (VERTEX_COORDINATE_COMPONENTS +
             NORMAL_COMPONENTS +
             TEXTURE_COORDINATE_COMPONENTS +
             NUMBER_OF_JOINT_INDICES) * BYTES_IN_FLOAT
    )
            GLES20.glEnableVertexAttribArray(jointWeightsAttribute)
    }*/

    if (auto mvpMatrixUniform = shaderProgramContainer.mvpMatrixUniform(); mvpMatrixUniform >= 0) {
        glm::mat4x4 mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;
        glUniformMatrix4fv(mvpMatrixUniform, 1, false, &mvpMatrix[0][0]);
    }

    /*shaderProgram.lightMvpMatrixUniform.takeIf { it >= 0 }?.let { lightMvpMatrixUniform ->
            tmpMatrix.set(lightProjectionMatrix)
            tmpMatrix.mul(lightViewMatrix)
            tmpMatrix.mul(lightModelMatrix)
            tmpMatrix.get(tmpFloatArray)
            GLES20.glUniformMatrix4fv(lightMvpMatrixUniform, 1, false, tmpFloatArray, 0)
    }

    shaderProgram.modelMatrixUniform.takeIf { it >= 0 }?.let { modelMatrixUniform ->
            modelMatrix.get(tmpFloatArray)
            GLES20.glUniformMatrix4fv(modelMatrixUniform, 1, false, tmpFloatArray, 0)
    }

    shaderProgram.biasMatrixUniform.takeIf { it >= 0 }?.let { biasMatrixUniform ->
            BIAS_MATRIX.get(tmpFloatArray)
            GLES20.glUniformMatrix4fv(biasMatrixUniform, 1, false, tmpFloatArray, 0)
    }*/

    if (auto diffuseColorUniform = shaderProgramContainer.diffuseColorUniform(); diffuseColorUniform >= 0) {
        glUniform4f(
                diffuseColorUniform,
                materialComponent->material().diffuseColor.x,
                materialComponent->material().diffuseColor.y,
                materialComponent->material().diffuseColor.z,
                materialComponent->material().diffuseColor.w
        );
    }

    if (auto useDiffuseColorUniform = shaderProgramContainer.useDiffuseColorUniform(); useDiffuseColorUniform >= 0) {
        glUniform1i(useDiffuseColorUniform, GL_TRUE);
    }

    if (
            auto hasSkeletalAnimationUniform = shaderProgramContainer.hasSkeletalAnimationUniform();
            hasSkeletalAnimationUniform >= 0
    ) {
        glUniform1i(hasSkeletalAnimationUniform, GL_FALSE);
    }

    /*val textureName = material.textureName
    if (textureName != null) {
        val textureInfo = texturesManager.findTexture(textureName)
                          ?: error("Texture not found for ${gameObject?.name}")

        shaderProgram.textureUniform.takeIf { it >= 0 }?.let { textureUniform ->
                GLES20.glActiveTexture(GLES20.GL_TEXTURE0)
                GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, textureInfo.texture)
                GLES20.glUniform1i(textureUniform, 0)
        }

        shaderProgram.useDiffuseColorUniform.glUniform1i(GLES20.GL_FALSE)
    } else {
        shaderProgram.useDiffuseColorUniform.glUniform1i(GLES20.GL_TRUE)
    }

    shaderProgram.receiveShadows.glUniform1i(material.receiveShadows.toGLBoolean())
    safeLet(shadowMapTextureInfo, shaderProgram.shadowMapUniform.takeIf { it >= 0 }) { textureInfo, shadowMapUniform ->
                GLES20.glActiveTexture(GLES20.GL_TEXTURE1)
        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, textureInfo.texture)
        GLES20.glUniform1i(shadowMapUniform, 1)
    }

    val hasSkeletalAnimation = safeLet(
            shaderProgram.jointTransformsUniform.takeIf { it >= 0 },
    gameObject?.getComponent(SkeletalAnimatorComponent::class.java)?.jointTransforms
    ) { jointTransformsUniform, jointTransforms ->
                jointTransforms.forEachIndexed { i, jointTransform ->
                    jointTransform?.get(tmpJointTransformsFloatArray, i * MATRIX_COMPONENTS)
        }
        GLES20.glUniformMatrix4fv(
                jointTransformsUniform,
                MAX_JOINTS,
                false,
                tmpJointTransformsFloatArray,
                0
        )
        true
    } ?: false
    shaderProgram.hasSkeletalAnimationUniform.glUniform1i(hasSkeletalAnimation.toGLBoolean())*/

    /*if (materialComponent.isDoubleSided) {
        GLES20.glDisable(GLES20.GL_CULL_FACE)
    } else {
        GLES20.glEnable(GLES20.GL_CULL_FACE)
    }

    val mode = if (materialComponent.isWireframe) {
        GLES20.GL_LINES
    } else {
        GLES20.GL_TRIANGLES
    }
    GLES20.glLineWidth(lineWidth)

    if (materialComponent.isTranslucent) {
        GLES20.glEnable(GLES20.GL_CULL_FACE)

        GLES20.glCullFace(GLES20.GL_FRONT)
        GLES20.glDrawElements(
                mode,
                iboInfo.numberOfIndices,
                GLES20.GL_UNSIGNED_SHORT,
                0
        )

        GLES20.glCullFace(GLES20.GL_BACK)
        GLES20.glDrawElements(
                mode,
                iboInfo.numberOfIndices,
                GLES20.GL_UNSIGNED_SHORT,
                0
        )
    } else {
        GLES20.glDrawElements(
                mode,
                iboInfo.numberOfIndices,
                GLES20.GL_UNSIGNED_SHORT,
                0
        )
    }*/

    glDrawElements(
            GL_TRIANGLES,
            iboInfo.numberOfIndices,
            GL_UNSIGNED_SHORT,
            reinterpret_cast<void*>(0)
    );

    if (vertexPositionAttribute >= 0) {
        glDisableVertexAttribArray(vertexPositionAttribute);
    }
    if (vertexNormalAttribute >= 0) {
        glDisableVertexAttribArray(vertexNormalAttribute);
    }
    if (vertexUvAttribute >= 0) {
        glDisableVertexAttribArray(vertexUvAttribute);
    }

    /*shaderProgram.jointIndicesAttribute.takeIf { it >= 0 }?.let { jointIndicesAttribute ->
            GLES20.glDisableVertexAttribArray(jointIndicesAttribute)
    }
    shaderProgram.jointWeightsAttribute.takeIf { it >= 0 }?.let { jointWeightsAttribute ->
            GLES20.glDisableVertexAttribArray(jointWeightsAttribute)
    }*/

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    m_openGlErrorDetector->checkOpenGLErrors("OpenGlMeshRendererComponent::render");
}

std::shared_ptr<GameObjectComponent> OpenGlMeshRendererComponent::clone() {
    auto clone = std::make_shared<OpenGlMeshRendererComponent>(
            m_layerNames,
            m_geometryBuffersStorage,
            m_openGlErrorDetector
    );
    clone->setEnabled(m_isEnabled);
    return clone;
}
