//
// Created by Igor Lapin on 13/08/2020.
//

#ifndef ENGINE_3D_BULLET_PHYSICS_ENGINE_H
#define ENGINE_3D_BULLET_PHYSICS_ENGINE_H


#include "PhysicsEngine.h"

class BulletPhysicsEngine : public PhysicsEngine {

public:
    BulletPhysicsEngine();
    BulletPhysicsEngine(const BulletPhysicsEngine&) = delete;
    BulletPhysicsEngine(BulletPhysicsEngine&&) = delete;

    virtual void setGravity(const glm::vec3 gravity);

    virtual void setPosition(const std::string& rigidBodyName, const glm::vec3& position);

    virtual void setRotation(const std::string& rigidBodyName, const glm::quat& rotation);

    virtual void addForce(const std::string& rigidBodyName, const glm::vec3& force);

    virtual void addTorque(const std::string& rigidBodyName, const glm::vec3& torque);

    virtual void setVelocityDirectly(const std::string& rigidBodyName, const glm::vec3& velocity);

    virtual void setVelocityViaMotor(const std::string& rigidBodyName, const glm::vec3& velocity);

    virtual void setAngularVelocityDirectly(const std::string& rigidBodyName, const glm::vec3& angularVelocity);

    virtual void setAngularVelocityViaMotor(const std::string& rigidBodyName, const glm::vec3& velocity);

    virtual void setRigidBodyEnabled(const std::string& rigidBodyName, bool isEnabled);

    virtual void
    createCylinderRigidBody(std::shared_ptr <GameObject> gameObject, std::string name, std::optional<float> massValue,
                            float radius, float length, const glm::vec3& position, const glm::quat& rotation,
                            float maxMotorForceX, float maxMotorForceY, float maxMotorForceZ, float maxMotorTorqueX,
                            float maxMotorTorqueY, float maxMotorTorqueZ);

    virtual void
    createSphereRigidBody(std::shared_ptr <GameObject> gameObject, std::string name, std::optional<float> massValue,
                          float radius, const glm::vec3& position, const glm::quat& rotation,
                          const glm::vec3& maxMotorForce, const glm::vec3& maxMotorTorque);

    virtual void
    createBoxRigidBody(std::shared_ptr <GameObject> gameObject, std::string name, std::optional<float> massValue,
                       const glm::vec3& size, const glm::vec3& position, const glm::quat& rotation,
                       float maxMotorForceX, float maxMotorForceY, float maxMotorForceZ, float maxMotorTorqueX,
                       float maxMotorTorqueY, float maxMotorTorqueZ);

    virtual void createCharacterCapsuleRigidBody(std::shared_ptr <GameObject> gameObject, std::string name,
                                                 std::optional<float> massValue, float radius, float length,
                                                 const glm::vec3& position, float maxMotorForceX, float maxMotorForceY,
                                                 float maxMotorForceZ);

    virtual void createTriMeshRigidBody(std::shared_ptr <GameObject> gameObject, std::string name, const Mesh& mesh,
                                        std::optional<float> massValue, const glm::vec3& position,
                                        const glm::quat& rotation);

    virtual void removeRigidBody(const std::string& rigidBodyName);

    virtual void update(float dt);

    virtual void getRigidBodyRotationAndPosition(const std::string& rigidBodyName, glm::mat4x4& destRotationMatrix,
                                                 glm::vec3& destPosition);

    virtual void reset();

    BulletPhysicsEngine& operator=(const BulletPhysicsEngine&) = delete;
    BulletPhysicsEngine& operator=(BulletPhysicsEngine&&) = delete;
};


#endif //ENGINE_3D_BULLET_PHYSICS_ENGINE_H
