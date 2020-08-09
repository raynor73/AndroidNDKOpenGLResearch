//
// Created by Igor Lapin on 09/08/2020.
//

#include "OdePhysicsEngine.h"

OdePhysicsEngine::OdePhysicsEngine() {
    initODE();
}

OdePhysicsEngine::~OdePhysicsEngine() {
    deinitODE();
}

void OdePhysicsEngine::initODE() {
    dInitODE2(0);
    m_physicsWorldID = dWorldCreate();
    m_physicsSpaceID = dSimpleSpaceCreate(nullptr);
}

void OdePhysicsEngine::deinitODE() {
    dWorldDestroy(m_physicsWorldID);
    dSpaceDestroy(m_physicsSpaceID);
    dCloseODE();
}

void OdePhysicsEngine::setGravity(const glm::vec3 gravity) {
    dWorldSetGravity(m_physicsWorldID, gravity.x, gravity.y, gravity.z);
}

void OdePhysicsEngine::setPosition(const std::string& rigidBodyName, const glm::vec3& position) {

}

void OdePhysicsEngine::setRotation(const std::string& rigidBodyName, const glm::quat& rotation) {

}

void OdePhysicsEngine::addForce(const std::string& rigidBodyName, const glm::vec3& force) {

}

void OdePhysicsEngine::addTorque(const std::string& rigidBodyName, const glm::vec3& torque) {

}

void OdePhysicsEngine::setVelocityDirectly(const std::string& rigidBodyName, const glm::vec3& velocity) {

}

void OdePhysicsEngine::setVelocityViaMotor(const std::string& rigidBodyName, const glm::vec3& velocity) {

}

void OdePhysicsEngine::setAngularVelocityDirectly(const std::string& rigidBodyName, const glm::vec3& angularVelocity) {

}

void OdePhysicsEngine::setAngularVelocityViaMotor(const std::string& rigidBodyName, const glm::vec3& velocity) {

}

void OdePhysicsEngine::createCylinderRigidBody(
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
) {

}

void OdePhysicsEngine::createSphereRigidBody(
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
) {

}

void OdePhysicsEngine::createBoxRigidBody(
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
) {

}

void OdePhysicsEngine::createCharacterCapsuleRigidBody(
        std::shared_ptr<GameObject> gameObject,
        std::string name,
        std::optional<float> massValue,
        float radius,
        float length,
        const glm::vec3& position,
        float maxMotorForceX,
        float maxMotorForceY,
        float maxMotorForceZ
) {

}

void
OdePhysicsEngine::createTriMeshRigidBody(
        std::shared_ptr<GameObject> gameObject,
        std::string name,
        const Mesh& mesh,
        std::optional<float> massValue,
        const glm::vec3& position,
        const glm::quat& rotation
) {

}

void OdePhysicsEngine::removeRigidBody(const std::string& rigidBodyName) {

}

void OdePhysicsEngine::update(float dt) {

}

void OdePhysicsEngine::getRigidBodyRotationAndPosition(
        const std::string& rigidBodyName,
        glm::mat4x3& destRotationMatrix,
        glm::vec3& destPosition
) {

}

void OdePhysicsEngine::reset() {

}
