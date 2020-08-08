//
// Created by Igor Lapin on 28/06/2020.
//

#include <glm/gtc/quaternion.hpp>
#include <engine_3d/TextComponent.h>
#include <engine_3d/Utils.h>
#include "RenderingEngineDevScene.h"

using namespace Engine3D::Utils;

void RenderingEngineDevScene::update(float dt) {
    m_fpsCalculator.update(dt);

    if (m_sceneCloser != nullptr) {
        m_sceneCloser->update();
    }

    auto fpsText = std::static_pointer_cast<TextComponent>(
            m_gameObjectsMap["fpsText"]->findComponent(TextComponent::TYPE_NAME)
    );

    {
        std::stringstream ss;
        ss  << "FPS: " << int(m_fpsCalculator.fps());
        fpsText->setText(ss.str());
    }

    boxAngleX += dt * 45;
    boxAngleY += dt * 45;
    boxAngleZ += dt * 45;

    box2AngleX += dt * 30;
    box2AngleY += dt * 30;
    box2AngleZ += dt * 30;

    auto rotationMatrix = glm::identity<glm::mat4>();
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(boxAngleZ), glm::vec3(0, 0, 1));
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(boxAngleX), glm::vec3(1, 0, 0));
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(boxAngleY), glm::vec3(0, 1, 0));
    auto rotationQuaternion = glm::quat_cast(rotationMatrix);
    m_boxTransform->setRotation(rotationQuaternion);

    rotationMatrix = glm::identity<glm::mat4>();
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(box2AngleZ), glm::vec3(0, 0, 1));
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(box2AngleX), glm::vec3(1, 0, 0));
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(box2AngleY), glm::vec3(0, 1, 0));
    rotationQuaternion = glm::quat_cast(rotationMatrix);
    m_box2Transform->setRotation(rotationQuaternion);
}

void RenderingEngineDevScene::restoreFromStateRepresentation(const std::string stateRepresentation) {
    Scene::restoreFromStateRepresentation(stateRepresentation);

    m_sceneCloser = std::make_shared<SceneCloser>(
            m_sceneManager,
            m_gameObjectsMap.at("closeButton"),
            SceneType::SCENES_SELECTION_SCENE
    );

    auto boxGameObject = m_gameObjectsMap.at("box");
    throwErrorIfNull(boxGameObject, "Box not found");
    m_boxTransform = std::static_pointer_cast<TransformationComponent>(boxGameObject->findComponent(
            TransformationComponent::TYPE_NAME
    ));
    throwErrorIfNull(m_boxTransform, "Box has not transform");

    auto box2GameObject = m_gameObjectsMap.at("box2");
    throwErrorIfNull(box2GameObject, "Box2 not found");
    m_box2Transform = std::static_pointer_cast<TransformationComponent>(box2GameObject->findComponent(
            TransformationComponent::TYPE_NAME
    ));
    throwErrorIfNull(m_box2Transform, "Box has not transform");
}
