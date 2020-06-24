#include "Entity.hpp"

Entity::Entity(Model* _model, PhysicsEntity* _pysicsEntity) {
    model = _model;
    pysicsEntity = _pysicsEntity;
    transform = pysicsEntity->rigidBody->getWorldTransform();

    // set mass
    // btVector3 inertia;
    // float massVal;
    // pysicsEntity->rigidBody->getCollisionShape()->calculateLocalInertia(massVal, inertia);
    // mass = massVal;
};

Entity::~Entity() {
    // delete entity model
    delete model;

    // delete rigidBody from physics simulation
    delete pysicsEntity;
};

//------------------------------------------------------------------------------
void Entity::update(float deltaTime) {
    // get physics
    // pysicsEntity->rigidBody->getMotionState()->getWorldTransform(transform);
    transform = pysicsEntity->rigidBody->getWorldTransform();
    btVector3 pos = transform.getOrigin();
    btQuaternion rot = transform.getRotation();

    // apply transforms to model
    model->setPos(glm::vec3(pos.getX(), pos.getY(), pos.getZ()));
    model->setRot(glm::quat(rot.getW(), rot.getX(), rot.getY(), rot.getZ()));
    model->updateTransform();

    // update animation
    model->updateAnimation(deltaTime);
};

//------------------------------------------------------------------------------
// handle transform
void Entity::setPos(glm::vec3 _position) {
    // set as kinematic
    setKinematic(true);

    // reset forces and velocities
    pysicsEntity->rigidBody->clearForces();
    pysicsEntity->rigidBody->setLinearVelocity(btVector3(0,0,0));
    pysicsEntity->rigidBody->setAngularVelocity(btVector3(0,0,0));

    // set pos
    transform.setOrigin(btVector3(_position.x, _position.y, _position.z));
    pysicsEntity->rigidBody->setCenterOfMassTransform(transform);
    pysicsEntity->rigidBody->getMotionState()->setWorldTransform(transform);

    // reset kinematic status
    setKinematic(kinematicTemp);
};
void Entity::setKinematicPos(glm::vec3 _position) {
    // set pos
    transform.setOrigin(btVector3(_position.x, _position.y, _position.z));
    pysicsEntity->rigidBody->setCenterOfMassTransform(transform);
    pysicsEntity->rigidBody->getMotionState()->setWorldTransform(transform);
};
glm::vec3 Entity::getPos() {
    btVector3 pos = pysicsEntity->rigidBody->getWorldTransform().getOrigin();
    // btVector3 pos = transform.getOrigin();
    return glm::vec3(pos.getX(), pos.getY(), pos.getZ());
};
void Entity::setRot(glm::quat _rotation) {
    // set as kinematic
    setKinematic(true);

    // reset forces and velocities
    pysicsEntity->rigidBody->clearForces();
    pysicsEntity->rigidBody->setLinearVelocity(btVector3(0,0,0));
    pysicsEntity->rigidBody->setAngularVelocity(btVector3(0,0,0));

    // set pos
    transform.setRotation(btQuaternion(_rotation.x, _rotation.y, _rotation.z, _rotation.w));
    pysicsEntity->rigidBody->setCenterOfMassTransform(transform);
    pysicsEntity->rigidBody->getMotionState()->setWorldTransform(transform);

    // reset kinematic status
    setKinematic(kinematicTemp);
};
void Entity::setKinematicRot(glm::quat _rotation) {
    // set pos
    transform.setRotation(btQuaternion(_rotation.x, _rotation.y, _rotation.z, _rotation.w));
    pysicsEntity->rigidBody->setCenterOfMassTransform(transform);
    pysicsEntity->rigidBody->getMotionState()->setWorldTransform(transform);
};
glm::quat Entity::getRot() {
    btQuaternion rot = transform.getRotation();
    return glm::quat(rot.getW(), rot.getX(), rot.getY(), rot.getZ());
};

//------------------------------------------------------------------------------
// handle physics
void Entity::setStaticFlag(bool state) {
    isStatic = state;
    setStatic(state);
};
void Entity::setStatic(bool state) {
    if (state) {
        pysicsEntity->rigidBody->setCollisionFlags(pysicsEntity->rigidBody->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
        // setKinematicFlag(true);
    } else {
        pysicsEntity->rigidBody->setCollisionFlags(pysicsEntity->rigidBody->getCollisionFlags() & ~btCollisionObject::CF_STATIC_OBJECT);
    };
};
bool Entity::getIsStatic() {
    return isStatic;
};

void Entity::setKinematicFlag(bool state) {
    isKinematic = state;
    // setKinematic(state);
};
void Entity::setKinematicTemp(bool state) {
    kinematicTemp = state;
    setKinematic(state);
};
void Entity::setKinematic(bool state) {
    if (state) {
        pysicsEntity->rigidBody->setCollisionFlags(pysicsEntity->rigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
    } else {
        pysicsEntity->rigidBody->setCollisionFlags(pysicsEntity->rigidBody->getCollisionFlags() & ~btCollisionObject::CF_KINEMATIC_OBJECT);
    };
};
bool Entity::getIsKinematic() {
    return isKinematic;
};
/*
void Entity::setMass(float _mass) {

    btVector3 inertia;
    float massVal;
    pysicsEntity->rigidBody->getCollisionShape()->calculateLocalInertia(massVal, inertia);

    mass = _mass;
    pysicsEntity->rigidBody->setMassProps(mass, inertia);

};
float Entity::getMass() {
    return mass;
};
*/
