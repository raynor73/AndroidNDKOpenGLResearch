//
// Created by Igor Lapin on 09/08/2020.
//

#ifndef ENGINE_3D_PHYSICSENGINE_H
#define ENGINE_3D_PHYSICSENGINE_H


#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <string>
#include <optional>
#include "Mesh.h"
#include "GameObject.h"

class PhysicsEngine {

public:
    virtual ~PhysicsEngine() = default;

    virtual void setGravity(const glm::vec3 gravity) = 0;

    virtual void setPosition(const std::string& rigidBodyName, const glm::vec3& position) = 0;
    virtual void setRotation(const std::string& rigidBodyName, const glm::quat& rotation) = 0;

    virtual void addForce(const std::string& rigidBodyName, const glm::vec3& force) = 0;
    virtual void addTorque(const std::string& rigidBodyName, const glm::vec3& torque) = 0;

    virtual void setVelocityDirectly(const std::string& rigidBodyName, const glm::vec3& velocity) = 0;
    virtual void setVelocityViaMotor(const std::string& rigidBodyName, const glm::vec3& velocity) = 0;

    virtual void setAngularVelocityDirectly(const std::string& rigidBodyName, const glm::vec3& angularVelocity) = 0;
    virtual void setAngularVelocityViaMotor(const std::string& rigidBodyName, const glm::vec3& velocity) = 0;

    virtual void setRigidBodyEnabled(const std::string& rigidBodyName, bool isEnabled) = 0;

    virtual void createCylinderRigidBody(
            std::shared_ptr<GameObject> gameObject,
            std::string name,
            std::optional<float> massValue,
            float radius,
            float length,
            const glm::vec3& position,
            const glm::quat& rotation,
            float maxMotorForceX, // TODO May be motor params should be passed via glm::vec3 class?
            float maxMotorForceY,
            float maxMotorForceZ,
            float maxMotorTorqueX,
            float maxMotorTorqueY,
            float maxMotorTorqueZ
    ) = 0;

    virtual void createSphereRigidBody(
            std::shared_ptr<GameObject> gameObject,
            std::string name,
            std::optional<float> massValue,
            float radius,
            const glm::vec3& position,
            const glm::quat& rotation,
            float maxMotorForceX,
            float maxMotorForceY,
            float maxMotorForceZ,
            float maxMotorTorqueX,
            float maxMotorTorqueY,
            float maxMotorTorqueZ
    ) = 0;

    virtual void createBoxRigidBody(
            std::shared_ptr<GameObject> gameObject,
            std::string name,
            std::optional<float> massValue,
            const glm::vec3& size,
            const glm::vec3& position,
            const glm::quat& rotation,
            float maxMotorForceX,
            float maxMotorForceY,
            float maxMotorForceZ,
            float maxMotorTorqueX,
            float maxMotorTorqueY,
            float maxMotorTorqueZ
    ) = 0;

    virtual void createCharacterCapsuleRigidBody(
            std::shared_ptr<GameObject> gameObject,
            std::string name,
            std::optional<float> massValue,
            float radius,
            float length,
            const glm::vec3& position,
            float maxMotorForceX,
            float maxMotorForceY,
            float maxMotorForceZ
    ) = 0;

    virtual void createTriMeshRigidBody(
            std::shared_ptr<GameObject> gameObject,
            std::string name,
            const Mesh& mesh,
            std::optional<float> massValue,
            const glm::vec3& position,
            const glm::quat& rotation
    ) = 0;

    virtual void removeRigidBody(const std::string& rigidBodyName) = 0;

    virtual void update(float dt) = 0;

    virtual void getRigidBodyRotationAndPosition(
            const std::string& rigidBodyName,
            glm::mat4x3& destRotationMatrix,
            glm::vec3& destPosition
    ) = 0;

    virtual void reset() = 0;
};


#endif //ENGINE_3D_PHYSICSENGINE_H
