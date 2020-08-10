//
// Created by Igor Lapin on 09/08/2020.
//

#ifndef ENGINE_3D_ODE_PHYSICS_ENGINE_H
#define ENGINE_3D_ODE_PHYSICS_ENGINE_H


#include <ode/ode.h>
#include <unordered_map>
#include <string>
#include <memory>
#include <utility>
#include <engine_3d/GameObject.h>
#include "PhysicsEngine.h"

/*namespace std {

    template<>
    struct hash<dGeomID> {
        size_t operator()(const dGeomID& value) const {
            size_t typeHash;
            size_t valueHash;
            if (std::holds_alternative<PercentValue>(complexValue)) {
                auto percentValue = std::get<PercentValue>(complexValue);

                typeHash = std::hash<std::string>{}("%");
                valueHash = std::hash<float>{}(percentValue.value);
            } else if (std::holds_alternative<DpValue>(complexValue)) {
                auto dpValue = std::get<DpValue>(complexValue);

                typeHash = std::hash<std::string>{}("dp");
                valueHash = std::hash<float>{}(dpValue.value);
            } else {
                auto plainValue = std::get<PlainValue>(complexValue);

                typeHash = std::hash<std::string>{}("px");
                valueHash = std::hash<float>{}(plainValue.value);
            }

            return typeHash ^ (valueHash << 1);
        }
    };
}*/

class OdePhysicsEngine : public PhysicsEngine {

    dWorldID m_physicsWorldID;
    dSpaceID m_physicsSpaceID;

    std::unordered_map<std::string, dBodyID> m_rigidBodies;
    std::unordered_map<std::string, dGeomID> m_collisionShapes;
    std::unordered_map<dGeomID, std::shared_ptr<GameObject>> m_collisionShapeToGameObjectMap;
    std::unordered_map<std::string, dJointID> m_linearMotors;
    std::unordered_map<std::string, dJointID> m_angularMotors;

public:
    OdePhysicsEngine();
    OdePhysicsEngine(const OdePhysicsEngine&) = delete;
    OdePhysicsEngine(OdePhysicsEngine&&) = delete;

    virtual ~OdePhysicsEngine() override;

    OdePhysicsEngine& operator=(const OdePhysicsEngine&) = delete;
    OdePhysicsEngine& operator=(OdePhysicsEngine&&) = delete;

    virtual void setGravity(const glm::vec3 gravity) override;

    virtual void setPosition(const std::string& rigidBodyName, const glm::vec3& position) override;

    virtual void setRotation(const std::string& rigidBodyName, const glm::quat& rotation) override;

    virtual void addForce(const std::string& rigidBodyName, const glm::vec3& force) override;

    virtual void addTorque(const std::string& rigidBodyName, const glm::vec3& torque) override;

    virtual void setVelocityDirectly(const std::string& rigidBodyName, const glm::vec3& velocity) override;

    virtual void setVelocityViaMotor(const std::string& rigidBodyName, const glm::vec3& velocity) override;

    virtual void setAngularVelocityDirectly(
            const std::string& rigidBodyName,
            const glm::vec3& angularVelocity
    ) override;

    virtual void setAngularVelocityViaMotor(const std::string& rigidBodyName, const glm::vec3& velocity) override;

    virtual void setRigidBodyEnabled(const std::string& rigidBodyName, bool isEnabled) override;

    virtual void createCylinderRigidBody(
            std::shared_ptr<GameObject> gameObject,
            std::string name,
            std::optional<float> massValue,
            float radius,
            float length,
            const glm::vec3& position,
            const glm::quat& rotation,
            float maxMotorForceX,
            float maxMotorForceY,
            float maxMotorForceZ,
            float maxMotorTorqueX,
            float maxMotorTorqueY,
            float maxMotorTorqueZ
    ) override;

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
    ) override;

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
    ) override;

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
    ) override;

    virtual void createTriMeshRigidBody(
            std::shared_ptr<GameObject> gameObject,
            std::string name,
            const Mesh& mesh,
            std::optional<float> massValue,
            const glm::vec3& position,
            const glm::quat& rotation
    ) override;

    virtual void removeRigidBody(const std::string& rigidBodyName) override;

    virtual void update(float dt) override;

    virtual void getRigidBodyRotationAndPosition(
            const std::string& rigidBodyName,
            glm::mat4x3& destRotationMatrix,
            glm::vec3& destPosition
    ) override;

    virtual void reset() override;

private:
    void initODE();
    void deinitODE();

    void glmVec3toDVector3(const glm::vec3& vector, dVector3& destVector) {
        destVector[0] = vector.x;
        destVector[1] = vector.y;
        destVector[2] = vector.z;
    }

    glm::vec3 dVector3ToGlmVec3(const dVector3& vector) {
        return glm::vec3(vector[0], vector[1], vector[2]);
    }

    void dVector3ToGlmVec3(const dVector3& vector, glm::vec3& destVector) {
        destVector.x = vector[0];
        destVector.y = vector[1];
        destVector.z = vector[2];
    }

    void glmQuatToDQuaternion(const glm::quat& quaternion, dQuaternion& destQuaternion) {
        destQuaternion[0] = quaternion.w;
        destQuaternion[1] = quaternion.x;
        destQuaternion[2] = quaternion.y;
        destQuaternion[3] = quaternion.z;
    }

    glm::quat dQuaternionToGlmQuat(const dQuaternion& quaternion) {
        return glm::quat(quaternion[0], quaternion[1], quaternion[2], quaternion[3]);
    }
};


#endif //ENGINE_3D_ODE_PHYSICS_ENGINE_H
