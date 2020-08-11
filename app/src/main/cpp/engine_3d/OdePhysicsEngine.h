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

class OdePhysicsEngine : public PhysicsEngine {

    static const int NUMBER_OF_MEANINGFUL_ODE_ROTATION_MATRIX_COLUMNS = 3;
    static const int TOTAL_NUMBER_OF_ODE_ROTATION_MATRIX_COLUMNS = 4;
    static const int TOTAL_NUMBER_OF_ODE_ROTATION_MATRIX_ROWS = 4;

    static constexpr float SIMULATION_STEP_TIME = 0.01f; // second
    static constexpr int MAX_SIMULATION_STEPS = 10;
    static const int MAX_CONTACTS = 64;

    dWorldID m_physicsWorldID;
    dSpaceID m_physicsSpaceID;
    dJointGroupID m_contactGroup;

    std::unordered_map<std::string, dBodyID> m_rigidBodies;
    std::unordered_map<std::string, dGeomID> m_collisionShapes;
    std::unordered_map<dGeomID, std::string> m_collisionShapeToGameObjectNameMap;
    std::unordered_map<std::string, dJointID> m_linearMotors;
    std::unordered_map<std::string, dJointID> m_angularMotors;

public:
    OdePhysicsEngine();
    OdePhysicsEngine(const OdePhysicsEngine&) = delete;
    OdePhysicsEngine(OdePhysicsEngine&&) = delete;

    friend void nearCallback(void *userData, dGeomID shape1, dGeomID shape2);

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
            const glm::vec3& maxMotorForce,
            const glm::vec3& maxMotorTorque
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
            glm::mat4x4& destRotationMatrix,
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

    void dVector3ToGlmVec3(const dReal* vector, glm::vec3& destVector) {
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

    dBodyID getRigidBody(const std::string& name);
};


#endif //ENGINE_3D_ODE_PHYSICS_ENGINE_H
