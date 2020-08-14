//
// Created by Igor Lapin on 13/08/2020.
//

#include "BulletPhysicsEngine.h"

BulletPhysicsEngine::BulletPhysicsEngine() {
    m_collisionConfiguration = new btDefaultCollisionConfiguration();
    m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);
    m_overlappingPairCache = new btDbvtBroadphase();
    m_solver = new btSequentialImpulseConstraintSolver();
    m_dynamicsWorld = new btDiscreteDynamicsWorld(
            m_dispatcher,
            m_overlappingPairCache, m_solver,
            m_collisionConfiguration
    );
}

BulletPhysicsEngine::~BulletPhysicsEngine() {
    delete m_dynamicsWorld;
    delete m_solver;
    delete m_overlappingPairCache;
    delete m_dispatcher;
    delete m_collisionConfiguration;
}

void BulletPhysicsEngine::setGravity(const glm::vec3& gravity) {
    m_dynamicsWorld->setGravity(glmVec3ToBtVector3(gravity));
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

void BulletPhysicsEngine::createSphereRigidBody(
        std::shared_ptr<GameObject> gameObject,
        std::string name,
        std::optional<float> massValue,
        float radius,
        const glm::vec3& position,
        const glm::quat& rotation,
        const glm::vec3& maxMotorForce,
        const glm::vec3& maxMotorTorque
) {
    auto shape = new btSphereShape(radius);

    auto btQuaternionRotation = glmQuatToBtQuaternion(rotation);
    auto btVector3Position = glmVec3ToBtVector3(position);

    auto motionState = new btDefaultMotionState(btTransform(btQuaternionRotation, btVector3Position));

    if (massValue) {
        btVector3 bodyInertia;
        shape->calculateLocalInertia(massValue.value(), bodyInertia);
        btRigidBody::btRigidBodyConstructionInfo bodyCI = btRigidBody::btRigidBodyConstructionInfo(
                massValue.value(),
                motionState,
                shape,
                bodyInertia
        );
        bodyCI.m_restitution = 1.0f;
        bodyCI.m_friction = 0.5f;

        auto body = std::make_shared<btRigidBody>(bodyCI);
        m_rigidBodies.insert({ name, body });

        m_dynamicsWorld->addRigidBody(body.get());
    } else {
        btVector3 bodyInertia;
        shape->calculateLocalInertia(0, bodyInertia);
        btRigidBody::btRigidBodyConstructionInfo bodyCI = btRigidBody::btRigidBodyConstructionInfo(
                0,
                motionState,
                shape,
                bodyInertia
        );
        bodyCI.m_restitution = 1.0f;
        bodyCI.m_friction = 0.5f;

        auto body = std::make_shared<btRigidBody>(bodyCI);
        m_rigidBodies.insert({ name, body });

        m_dynamicsWorld->addRigidBody(body.get());
    }
    //_body->setUserPointer((__bridge void*)self);
    //body->setLinearFactor(btVector3(1,1,0));

    /*if (_body)
    {
        delete _body->getMotionState();
        delete _body;
    }

    delete _shape;*/
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
BulletPhysicsEngine::createTriMeshRigidBody(
        std::shared_ptr<GameObject> gameObject,
        std::string name,
        const Mesh& mesh,
        std::optional<float> massValue,
        const glm::vec3& position,
        const glm::quat& rotation
) {
    auto btMesh = new btTriangleMesh(true, false);
    auto vertices = mesh.vertices();
    auto indices = mesh.indices();
    for (int i = 0; i < mesh.indices().size() / 3; i++) {
        btMesh->addTriangle(
                glmVec3ToBtVector3(vertices[indices[i * 3 + 0]].position()),
                glmVec3ToBtVector3(vertices[indices[i * 3 + 1]].position()),
                glmVec3ToBtVector3(vertices[indices[i * 3 + 2]].position()),
                true
        );
    }
    auto shape = new btBvhTriangleMeshShape(btMesh, true);

    auto btQuaternionRotation = glmQuatToBtQuaternion(rotation);
    auto btVector3Position = glmVec3ToBtVector3(position);

    auto motionState = new btDefaultMotionState(btTransform(btQuaternionRotation, btVector3Position));

    if (massValue) {
        btVector3 bodyInertia;
        shape->calculateLocalInertia(massValue.value(), bodyInertia);
        btRigidBody::btRigidBodyConstructionInfo bodyCI = btRigidBody::btRigidBodyConstructionInfo(
                massValue.value(),
                motionState,
                shape,
                bodyInertia
        );
        bodyCI.m_restitution = 1.0f;
        bodyCI.m_friction = 0.5f;

        auto body = std::make_shared<btRigidBody>(bodyCI);
        m_rigidBodies.insert({ name, body });

        m_dynamicsWorld->addRigidBody(body.get());
    } else {
        btVector3 bodyInertia;
        shape->calculateLocalInertia(0, bodyInertia);
        btRigidBody::btRigidBodyConstructionInfo bodyCI = btRigidBody::btRigidBodyConstructionInfo(
                0,
                motionState,
                shape,
                bodyInertia
        );
        bodyCI.m_restitution = 1.0f;
        bodyCI.m_friction = 0.5f;

        auto body = std::make_shared<btRigidBody>(bodyCI);
        m_rigidBodies.insert({ name, body });

        m_dynamicsWorld->addRigidBody(body.get());
    }
}

void BulletPhysicsEngine::removeRigidBody(const std::string& rigidBodyName) {

}

void BulletPhysicsEngine::update(float dt) {
    m_dynamicsWorld->stepSimulation(dt, 10);
}

void
BulletPhysicsEngine::getRigidBodyRotationAndPosition(const std::string& rigidBodyName, glm::mat4x4& destRotationMatrix,
                                                     glm::vec3& destPosition) {
    auto body = getRigidBody(rigidBodyName);
    auto transform = body->getWorldTransform();

    destPosition = btVector3ToGlmVec3(transform.getOrigin());
    destRotationMatrix = glm::mat4_cast(btQuaternionToGlmQuat(transform.getRotation()));
}

void BulletPhysicsEngine::reset() {

}

std::shared_ptr<btRigidBody> BulletPhysicsEngine::getRigidBody(const std::string& name) const {
    if (m_rigidBodies.count(name) == 0) {
        std::stringstream ss;
        ss << "Rigid body " << name << " not found";
        throw std::domain_error(ss.str());
    }

    return m_rigidBodies.at(name);
}
