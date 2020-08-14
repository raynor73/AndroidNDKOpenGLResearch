//
// Created by Igor Lapin on 13/08/2020.
//

#ifndef ENGINE_3D_BULLET_PHYSICS_ENGINE_H
#define ENGINE_3D_BULLET_PHYSICS_ENGINE_H


#include <string>
#include <memory>
#include <exception>
#include <unordered_map>
#include <bullet/btBulletDynamicsCommon.h>
#include "PhysicsEngine.h"

class BulletPhysicsEngine : public PhysicsEngine {

    btDefaultCollisionConfiguration* m_collisionConfiguration;
    btCollisionDispatcher* m_dispatcher;
    btDbvtBroadphase* m_overlappingPairCache;
    btSequentialImpulseConstraintSolver* m_solver;
    btDiscreteDynamicsWorld* m_dynamicsWorld;

    std::unordered_map<std::string, std::shared_ptr<btRigidBody>> m_rigidBodies;
    std::unordered_map<const btRigidBody*, GameObject*> m_btRigidBodyToGameObjectMap;

public:
    BulletPhysicsEngine();
    virtual ~BulletPhysicsEngine() override;
    BulletPhysicsEngine(const BulletPhysicsEngine&) = delete;
    BulletPhysicsEngine(BulletPhysicsEngine&&) = delete;

    virtual void setGravity(const glm::vec3& gravity) override;

    virtual void setPosition(const std::string& rigidBodyName, const glm::vec3& position) override;

    virtual void setRotation(const std::string& rigidBodyName, const glm::quat& rotation) override;

    virtual void addForce(const std::string& rigidBodyName, const glm::vec3& force) override;

    virtual void addTorque(const std::string& rigidBodyName, const glm::vec3& torque) override;

    virtual void setVelocityDirectly(const std::string& rigidBodyName, const glm::vec3& velocity) override;

    virtual void setVelocityViaMotor(const std::string& rigidBodyName, const glm::vec3& velocity) override;

    virtual void setAngularVelocityDirectly(const std::string& rigidBodyName, const glm::vec3& angularVelocity) override;

    virtual void setAngularVelocityViaMotor(const std::string& rigidBodyName, const glm::vec3& velocity) override;

    virtual void setRigidBodyEnabled(const std::string& rigidBodyName, bool isEnabled) override;

    virtual void
    createCylinderRigidBody(std::shared_ptr <GameObject> gameObject, std::string name, std::optional<float> massValue,
                            float radius, float length, const glm::vec3& position, const glm::quat& rotation,
                            float maxMotorForceX, float maxMotorForceY, float maxMotorForceZ, float maxMotorTorqueX,
                            float maxMotorTorqueY, float maxMotorTorqueZ) override;

    virtual void
    createSphereRigidBody(std::shared_ptr <GameObject> gameObject, std::string name, std::optional<float> massValue,
                          float radius, const glm::vec3& position, const glm::quat& rotation,
                          const glm::vec3& maxMotorForce, const glm::vec3& maxMotorTorque) override;

    virtual void
    createBoxRigidBody(std::shared_ptr <GameObject> gameObject, std::string name, std::optional<float> massValue,
                       const glm::vec3& size, const glm::vec3& position, const glm::quat& rotation,
                       float maxMotorForceX, float maxMotorForceY, float maxMotorForceZ, float maxMotorTorqueX,
                       float maxMotorTorqueY, float maxMotorTorqueZ) override;

    virtual void createCharacterCapsuleRigidBody(std::shared_ptr <GameObject> gameObject, std::string name,
                                                 std::optional<float> massValue, float radius, float length,
                                                 const glm::vec3& position, float maxMotorForceX, float maxMotorForceY,
                                                 float maxMotorForceZ) override;

    virtual void createTriMeshRigidBody(std::shared_ptr <GameObject> gameObject, std::string name, const Mesh& mesh,
                                        std::optional<float> massValue, const glm::vec3& position,
                                        const glm::quat& rotation) override;

    virtual void removeRigidBody(const std::string& rigidBodyName) override;

    virtual void update(float dt) override;

    virtual void getRigidBodyRotationAndPosition(const std::string& rigidBodyName, glm::mat4x4& destRotationMatrix,
                                                 glm::vec3& destPosition) override;

    virtual void reset() override;

    friend void tickCallback(btDynamicsWorld* world, btScalar timeStep);

    BulletPhysicsEngine& operator=(const BulletPhysicsEngine&) = delete;
    BulletPhysicsEngine& operator=(BulletPhysicsEngine&&) = delete;

private:
    void initBulletPhysics();
    void deinitBulletPhysics();

    void removeAllRigidBodies();

    std::shared_ptr<btRigidBody> getRigidBody(const std::string& name) const;

    static btVector3 glmVec3ToBtVector3(const glm::vec3& vector) {
        return btVector3(vector.x, vector.y, vector.z);
    }

    static btQuaternion glmQuatToBtQuaternion(const glm::quat& quaternion) {
        return btQuaternion(quaternion.x, quaternion.y, quaternion.z, quaternion.w);
    }

    static glm::vec3 btVector3ToGlmVec3(const btVector3& vector) {
        return glm::vec3(vector.x(), vector.y(), vector.z());
    }

    static glm::quat btQuaternionToGlmQuat(const btQuaternion& quaternion) {
        return glm::quat(quaternion.w(), quaternion.x(), quaternion.y(), quaternion.z());
    }
};


#endif //ENGINE_3D_BULLET_PHYSICS_ENGINE_H
