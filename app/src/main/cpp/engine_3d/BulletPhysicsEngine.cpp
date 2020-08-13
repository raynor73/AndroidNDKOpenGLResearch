//
// Created by Igor Lapin on 13/08/2020.
//

#include <bullet/btBulletDynamicsCommon.h>
#include "BulletPhysicsEngine.h"

BulletPhysicsEngine::BulletPhysicsEngine() {
    btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
}

void BulletPhysicsEngine::setGravity(const glm::vec3 gravity) {

}

void BulletPhysicsEngine::setPosition(const std::string& rigidBodyName, const glm::vec3& position) {

}

void BulletPhysicsEngine::setRotation(const std::string& rigidBodyName, const glm::quat& rotation) {

}

void BulletPhysicsEngine::addForce(const std::string& rigidBodyName, const glm::vec3& force) {

}

void BulletPhysicsEngine::addTorque(const std::string& rigidBodyName, const glm::vec3& torque) {

}

void BulletPhysicsEngine::setVelocityDirectly(const std::string& rigidBodyName, const glm::vec3& velocity) {

}

void BulletPhysicsEngine::setVelocityViaMotor(const std::string& rigidBodyName, const glm::vec3& velocity) {

}

void
BulletPhysicsEngine::setAngularVelocityDirectly(const std::string& rigidBodyName, const glm::vec3& angularVelocity) {

}

void BulletPhysicsEngine::setAngularVelocityViaMotor(const std::string& rigidBodyName, const glm::vec3& velocity) {

}

void BulletPhysicsEngine::setRigidBodyEnabled(const std::string& rigidBodyName, bool isEnabled) {

}

void BulletPhysicsEngine::createCylinderRigidBody(std::shared_ptr<GameObject> gameObject, std::string name,
                                                  std::optional<float> massValue, float radius, float length,
                                                  const glm::vec3& position, const glm::quat& rotation,
                                                  float maxMotorForceX, float maxMotorForceY, float maxMotorForceZ,
                                                  float maxMotorTorqueX, float maxMotorTorqueY, float maxMotorTorqueZ) {

}

void BulletPhysicsEngine::createSphereRigidBody(std::shared_ptr<GameObject> gameObject, std::string name,
                                                std::optional<float> massValue, float radius, const glm::vec3& position,
                                                const glm::quat& rotation, const glm::vec3& maxMotorForce,
                                                const glm::vec3& maxMotorTorque) {

}

void BulletPhysicsEngine::createBoxRigidBody(std::shared_ptr<GameObject> gameObject, std::string name,
                                             std::optional<float> massValue, const glm::vec3& size,
                                             const glm::vec3& position, const glm::quat& rotation, float maxMotorForceX,
                                             float maxMotorForceY, float maxMotorForceZ, float maxMotorTorqueX,
                                             float maxMotorTorqueY, float maxMotorTorqueZ) {

}

void BulletPhysicsEngine::createCharacterCapsuleRigidBody(std::shared_ptr<GameObject> gameObject, std::string name,
                                                          std::optional<float> massValue, float radius, float length,
                                                          const glm::vec3& position, float maxMotorForceX,
                                                          float maxMotorForceY, float maxMotorForceZ) {

}

void
BulletPhysicsEngine::createTriMeshRigidBody(std::shared_ptr<GameObject> gameObject, std::string name, const Mesh& mesh,
                                            std::optional<float> massValue, const glm::vec3& position,
                                            const glm::quat& rotation) {

}

void BulletPhysicsEngine::removeRigidBody(const std::string& rigidBodyName) {

}

void BulletPhysicsEngine::update(float dt) {

}

void
BulletPhysicsEngine::getRigidBodyRotationAndPosition(const std::string& rigidBodyName, glm::mat4x4& destRotationMatrix,
                                                     glm::vec3& destPosition) {

}

void BulletPhysicsEngine::reset() {

}
