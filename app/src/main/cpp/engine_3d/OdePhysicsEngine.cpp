//
// Created by Igor Lapin on 09/08/2020.
//

#include <sstream>
#include <exception>
#include <algorithm>
#include <engine_3d/Utils.h>
#include <main/L.h>
#include <main/Constants.h>
#include "OdePhysicsEngine.h"

using namespace Engine3D::Utils;

void nearCallback(void *userData, dGeomID shape1, dGeomID shape2) {
    auto physicsEngine = reinterpret_cast<OdePhysicsEngine*>(userData);
    dContact contactsBuffer[OdePhysicsEngine::MAX_CONTACTS];

    auto body1 = dGeomGetBody(shape1);
    auto body2 = dGeomGetBody(shape2);
    if (body1 && body2 && dAreConnectedExcluding(body1, body2, dJointTypeContact)) {
        return;
    }

    for (size_t i = 0; i < OdePhysicsEngine::MAX_CONTACTS; i++) {
        auto contact = contactsBuffer[i];
        contact.surface.mode = dContactBounce | dContactSoftCFM;
        contact.surface.mu = 50.0;
        contact.surface.mu2 = 50.0;
        contact.surface.bounce = 0.1;
        contact.surface.bounce_vel = 0.1;
        contact.surface.soft_cfm = 0.01;
    }

    auto numberOfCollisions = dCollide(
            shape1,
            shape2,
            OdePhysicsEngine::MAX_CONTACTS,
            &contactsBuffer[0].geom,
            sizeof(dContact)
    );

    for (size_t i = 0; i < numberOfCollisions; i++) {
        auto contactJoint = dJointCreateContact(
                physicsEngine->m_physicsWorldID,
                physicsEngine->m_contactGroup,
                &contactsBuffer[i]
        );
        dJointAttach(contactJoint, dGeomGetBody(contactsBuffer[i].geom.g1), dGeomGetBody(contactsBuffer[i].geom.g2));
        /*val gameObject1 = gameObjects[o1] ?: error("No game object found #1")
        val gameObject2 = gameObjects[o2] ?: error("No game object found #2")

        gameObject1.getComponent(CollisionsInfoComponent::class.java)?.let {
                it.collisions += CollisionsInfoComponent.Collision(
                        gameObject2,
                        contact.contactGeom.pos.toVector(),
                        contact.contactGeom.normal.toVector(),
                        contact.contactGeom.depth.toFloat()
                )
        }

        gameObject2.getComponent(CollisionsInfoComponent::class.java)?.let {
                it.collisions += CollisionsInfoComponent.Collision(
                        gameObject1,
                        contact.contactGeom.pos.toVector(),
                        contact.contactGeom.normal.toVector(),
                        contact.contactGeom.depth.toFloat()
                )
        }*/
    }
}

OdePhysicsEngine::OdePhysicsEngine() {
    dInitODE2(0);
    initODE();
}

OdePhysicsEngine::~OdePhysicsEngine() {
    deinitODE();
    dCloseODE();
}

void OdePhysicsEngine::initODE() {
    //dInitODE2(0);
    m_physicsWorldID = dWorldCreate();
    m_physicsSpaceID = dSimpleSpaceCreate(nullptr);
    m_contactGroup = dJointGroupCreate(MAX_CONTACTS);
    dWorldSetCFM(m_physicsWorldID, 1e-5);
}

void OdePhysicsEngine::deinitODE() {
    dWorldDestroy(m_physicsWorldID);
    dSpaceDestroy(m_physicsSpaceID);
    dJointGroupEmpty(m_contactGroup);
    dJointGroupDestroy(m_contactGroup);
    //dCloseODE();
}

void OdePhysicsEngine::setGravity(const glm::vec3 gravity) {
    dWorldSetGravity(m_physicsWorldID, gravity.x, gravity.y, gravity.z);
}

void OdePhysicsEngine::setPosition(const std::string& rigidBodyName, const glm::vec3& position) {
    dBodySetPosition(getRigidBody(rigidBodyName), position.x, position.y, position.z);
}

void OdePhysicsEngine::setRotation(const std::string& rigidBodyName, const glm::quat& rotation) {
    dQuaternion quaternion = { rotation.w, rotation.x, rotation.y, rotation.z };
    dBodySetQuaternion(getRigidBody(rigidBodyName), quaternion);
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
    auto rigidBody = getRigidBody(rigidBodyName);
    if (isEnabled) {
        dBodyEnable(rigidBody);
        /*dBodySetAutoDisableFlag(rigidBody, true);
        dBodySetAutoDisableDefaults(rigidBody);*/
    } else {
        dBodyDisable(rigidBody);
    }
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
        const glm::vec3& maxMotorForce,
        const glm::vec3& maxMotorTorque
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
    m_collisionShapeToGameObjectNameMap.insert({collisionShape, gameObject->name() });

    auto motor = dJointCreateLMotor(m_physicsWorldID, nullptr);
    dJointSetLMotorNumAxes(motor, 3);
    dJointSetLMotorAxis(motor, 0, 0, 1, 0, 0);
    dJointSetLMotorAxis(motor, 1, 0, 0, 1, 0);
    dJointSetLMotorAxis(motor, 2, 0, 0, 0, 1);
    dJointSetLMotorParam(motor, dParamFMax, maxMotorForce.x);
    dJointSetLMotorParam(motor, dParamFMax2, maxMotorForce.y);
    dJointSetLMotorParam(motor, dParamFMax3, maxMotorForce.z);
    dJointSetLMotorParam(motor, dParamVel, 0);
    dJointSetLMotorParam(motor, dParamVel2, 0);
    dJointSetLMotorParam(motor, dParamVel3, 0);
    //dJointAttach(motor, rigidBody, nullptr);
    m_linearMotors.insert({ name, motor });

    auto angularMotor = dJointCreateAMotor(m_physicsWorldID, nullptr);
    dJointSetAMotorNumAxes(angularMotor, 3);
    dJointSetAMotorAxis(angularMotor, 0, 0, 1, 0, 0);
    dJointSetAMotorAxis(angularMotor, 1, 0, 0, 1, 0);
    dJointSetAMotorAxis(angularMotor, 2, 0, 0, 0, 1);
    dJointSetAMotorParam(angularMotor, dParamFMax, maxMotorTorque.x);
    dJointSetAMotorParam(angularMotor, dParamFMax2, maxMotorTorque.y);
    dJointSetAMotorParam(angularMotor, dParamFMax3, maxMotorTorque.z);
    dJointSetAMotorParam(angularMotor, dParamVel, 0);
    dJointSetAMotorParam(angularMotor, dParamVel2, 0);
    dJointSetAMotorParam(angularMotor, dParamVel3, 0);
    //dJointAttach(angularMotor, rigidBody, nullptr);
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

void OdePhysicsEngine::createTriMeshRigidBody(
        std::shared_ptr<GameObject> gameObject,
        std::string name,
        const Mesh& mesh,
        std::optional<float> massValue,
        const glm::vec3& position,
        const glm::quat& rotation
) {
    if (m_rigidBodies.count(name) > 0) {
        std::stringstream ss;
        ss << "Already has " << name << " rigid body";
        throw std::domain_error(ss.str());
    }

    auto triMeshData = dGeomTriMeshDataCreate();
    auto vertexData = new dReal[mesh.vertices().size() * Vertex::VERTEX_POSITION_COMPONENTS];
    auto indexData = new dTriIndex[mesh.indices().size()];
    for (size_t i = 0; i < mesh.vertices().size(); i++) {
        vertexData[i * Vertex::VERTEX_POSITION_COMPONENTS + 0] = mesh.vertices()[i].position().x;
        vertexData[i * Vertex::VERTEX_POSITION_COMPONENTS + 1] = mesh.vertices()[i].position().y;
        vertexData[i * Vertex::VERTEX_POSITION_COMPONENTS + 2] = mesh.vertices()[i].position().z;
    }
    for (size_t i = 0; i < mesh.indices().size(); i++) {
        indexData[i] = mesh.indices()[i];
    }
    //dGeomTriMeshDataBuildSimple(triMeshData, vertexData, mesh.vertices().size(), indexData, mesh.indices().size());
    dGeomTriMeshDataBuildSingle(
            triMeshData,
            vertexData,
            Vertex::VERTEX_POSITION_COMPONENTS * sizeof(float),
            mesh.vertices().size(),
            indexData,
            mesh.indices().size(),
            3 * sizeof(dTriIndex)
    );
    //delete[] vertexData;
    //delete[] indexData;

    //dGeomTriMeshDataPreprocess(triMeshData);
    dGeomTriMeshDataPreprocess2(triMeshData, 1U << dTRIDATAPREPROCESS_BUILD_FACE_ANGLES, nullptr);

    auto triMesh = dCreateTriMesh(m_physicsSpaceID, triMeshData, nullptr, nullptr, nullptr);
    m_collisionShapes.insert({ name, triMesh });
    m_collisionShapeToGameObjectNameMap.insert({ triMesh, gameObject->name() });

    dMass mass;

    auto rigidBody = dBodyCreate(m_physicsWorldID);
    m_rigidBodies.insert({ name, rigidBody });

    if (massValue) {
        mass.setTrimeshTotal(massValue.value(), triMesh);
        dBodySetMass(rigidBody, &mass);
    } else {
        dBodySetKinematic(rigidBody);
    }

    dGeomSetBody(triMesh, rigidBody);

    dBodySetPosition(rigidBody, position.x, position.y, position.z);
    dQuaternion quaternion;
    glmQuatToDQuaternion(rotation, quaternion);
    dBodySetQuaternion(rigidBody, quaternion);
}

void OdePhysicsEngine::removeRigidBody(const std::string& rigidBodyName) {
    if (m_linearMotors.count(rigidBodyName) == 0) {
        std::stringstream ss;
        ss << "Linear motor for " << rigidBodyName << " not found";
        //throw std::domain_error(ss.str());
        L::w(App::Constants::LOG_TAG, ss.str());
    } else {
        dJointDestroy(m_linearMotors.at(rigidBodyName));
        m_linearMotors.erase(rigidBodyName);
    }

    if (m_angularMotors.count(rigidBodyName) == 0) {
        std::stringstream ss;
        ss << "Angular motor for " << rigidBodyName << " not found";
        //throw std::domain_error(ss.str());
        L::w(App::Constants::LOG_TAG, ss.str());
    } else {
        dJointDestroy(m_angularMotors.at(rigidBodyName));
        m_angularMotors.erase(rigidBodyName);
    }

    if (m_collisionShapes.count(rigidBodyName) == 0) {
        std::stringstream ss;
        ss << "Collision shape for " << rigidBodyName << " not found";
        throw std::domain_error(ss.str());
    }
    auto collisionShape = m_collisionShapes.at(rigidBodyName);
    m_collisionShapeToGameObjectNameMap.erase(collisionShape);
    dGeomDestroy(collisionShape);
    m_collisionShapes.erase(rigidBodyName);

    //characterCapsules.remove(rigidBodyName)

    dBodyDestroy(getRigidBody(rigidBodyName));
    m_rigidBodies.erase(rigidBodyName);
}

void OdePhysicsEngine::update(float dt) {
    for (int i = 0; i < int(ceil(dt / SIMULATION_STEP_TIME)); i++) {
        dSpaceCollide(m_physicsSpaceID, this, nearCallback);

        dWorldStep(m_physicsWorldID, SIMULATION_STEP_TIME);

        dJointGroupEmpty(m_contactGroup);
    }

    /*for (int i = 0; i < std::min(int(ceil(dt / SIMULATION_STEP_TIME)), MAX_SIMULATION_STEPS); i++) {
        dSpaceCollide(m_physicsSpaceID, this, nearCallback);

        dWorldStep(m_physicsWorldID, SIMULATION_STEP_TIME);

        dJointGroupEmpty(m_contactGroup);
    }*/

    /*val collisionInfoContainers = gameObjects
            .values
            .mapNotNull { it.getComponent(CollisionsInfoComponent::class.java) }

    repeat(min(ceil(dt / SIMULATION_STEP_TIME).toInt(), MAX_SIMULATION_STEPS)) {
        collisionInfoContainers.forEach { it.collisions.clear() }

        OdeHelper.spaceCollide(space, null, this)
        world?.step(SIMULATION_STEP_TIME)
        characterCapsules.values.forEach {
            tmpQuaternion.identity().rotateX(-(PI / 2).toFloat()).toQuaternion(tmpDQuaternion)
            it.quaternion = tmpDQuaternion
        }
        contactGroup.empty()
    }*/
}

void OdePhysicsEngine::getRigidBodyRotationAndPosition(
        const std::string& rigidBodyName,
        glm::mat4x4& destRotationMatrix,
        glm::vec3& destPosition
) {
    auto rigidBody = getRigidBody(rigidBodyName);

    auto rotationMatrix3x4 = dBodyGetRotation(rigidBody);
    destRotationMatrix = glm::identity<glm::mat4x4>();
    for (int i = 0; i < TOTAL_NUMBER_OF_ODE_ROTATION_MATRIX_ROWS; i++) {
        std::memcpy(
                &destRotationMatrix[0][i],
                &rotationMatrix3x4[TOTAL_NUMBER_OF_ODE_ROTATION_MATRIX_COLUMNS * i],
                sizeof(dReal) * NUMBER_OF_MEANINGFUL_ODE_ROTATION_MATRIX_COLUMNS
        );
    }
    destRotationMatrix[3][3] = 1;
    destRotationMatrix = glm::transpose(destRotationMatrix);

    dVector3ToGlmVec3(dBodyGetPosition(rigidBody), destPosition);
}

void OdePhysicsEngine::reset() {
    deinitODE();

    //characterCapsules.clear()
    m_rigidBodies.clear();
    //contactGroup.clear()
    m_collisionShapes.clear();
    m_collisionShapeToGameObjectNameMap.clear();
    m_linearMotors.clear();
    m_angularMotors.clear();

    initODE();
}

dBodyID OdePhysicsEngine::getRigidBody(const std::string& name) {
    if (m_rigidBodies.count(name) == 0) {
        std::stringstream ss;
        ss << "Rigid body " << name << " not found";
        throw std::domain_error(ss.str());
    }

    return m_rigidBodies.at(name);
}
