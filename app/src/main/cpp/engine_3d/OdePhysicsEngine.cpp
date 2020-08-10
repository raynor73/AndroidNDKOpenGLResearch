//
// Created by Igor Lapin on 09/08/2020.
//

#include <sstream>
#include <exception>
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

void OdePhysicsEngine::setRigidBodyEnabled(const std::string& rigidBodyName, bool isEnabled) {

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
    if (m_rigidBodies.count(name) > 0) {
        std::stringstream ss;
        ss << "Already has " << name << " rigid body";
        throw std::domain_error(ss.str());
    }

    dMass mass;

    auto rigidBody = dBodyCreate(m_physicsWorldID);
    m_rigidBodies.insert({ name, rigidBody });

    if (massValue) {
        mass.setSphereTotal(massValue.value(), radius);
        dBodySetMass(rigidBody, &mass);
    } else {
        dBodySetKinematic(rigidBody);
    }

    auto collisionShape = dCreateSphere(nullptr, radius);
    m_collisionShapes.insert({ name, collisionShape });
    dGeomSetBody(collisionShape, rigidBody);

    dBodySetPosition(rigidBody, position.x, position.y, position.z);
    dQuaternion quaternion;
    glmQuatToDQuaternion(rotation, quaternion);
    dBodySetQuaternion(rigidBody, quaternion);

    dSpaceAdd(m_physicsSpaceID, collisionShape);
    m_collisionShapeToGameObjectMap.insert({ collisionShape, gameObject });

    auto motor = dJointCreateLMotor(m_physicsWorldID, nullptr);
    dJointSetLMotorNumAxes(motor, 3);
    dJointSetLMotorAxis(motor, 0, 0, 1, 0, 0);
    dJointSetLMotorAxis(motor, 1, 0, 0, 1, 0);
    dJointSetLMotorAxis(motor, 2, 0, 0, 0, 1);
    dJointSetLMotorParam(motor, dParamFMax, maxMotorForceX);
    dJointSetLMotorParam(motor, dParamFMax2, maxMotorForceY);
    dJointSetLMotorParam(motor, dParamFMax3, maxMotorForceZ);
    dJointSetLMotorParam(motor, dParamVel, 0);
    dJointSetLMotorParam(motor, dParamVel2, 0);
    dJointSetLMotorParam(motor, dParamVel3, 0);
    dJointAttach(motor, rigidBody, nullptr);
    m_linearMotors.insert({ name, motor });

    auto angularMotor = dJointCreateAMotor(m_physicsWorldID, nullptr);
    dJointSetAMotorNumAxes(angularMotor, 3);
    dJointSetAMotorAxis(angularMotor, 0, 0, 1, 0, 0);
    dJointSetAMotorAxis(angularMotor, 1, 0, 0, 1, 0);
    dJointSetAMotorAxis(angularMotor, 2, 0, 0, 0, 1);
    dJointSetAMotorParam(angularMotor, dParamFMax, maxMotorTorqueX);
    dJointSetAMotorParam(angularMotor, dParamFMax2, maxMotorTorqueY);
    dJointSetAMotorParam(angularMotor, dParamFMax3, maxMotorTorqueZ);
    dJointSetAMotorParam(angularMotor, dParamVel, 0);
    dJointSetAMotorParam(angularMotor, dParamVel2, 0);
    dJointSetAMotorParam(angularMotor, dParamVel3, 0);
    dJointAttach(angularMotor, rigidBody, nullptr);
    m_angularMotors.insert({ name, angularMotor });
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
