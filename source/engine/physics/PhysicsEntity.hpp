#ifndef SAH_PHYSICS_ENTITY
#define SAH_PHYSICS_ENTITY

#include "Physics.hpp"

class PhysicsEntity {
private:

public:
    btRigidBody* rigidBody;

    PhysicsEntity() {};
    virtual ~PhysicsEntity() {
        // delete rigidBody from physics simulation
        Physics::deleteBody(rigidBody);
    };
};

class PhysicsEntity_Box : public PhysicsEntity {
private:

public:
    PhysicsEntity_Box(btVector3 size, float mass, btVector3 pos) {
        rigidBody = Physics::addBox(size, mass, pos);
    };
};
class PhysicsEntity_Sphere : public PhysicsEntity {
private:

public:
    PhysicsEntity_Sphere(float radius, float mass, btVector3 pos) {
        rigidBody = Physics::addSphere(radius, mass, pos);
    };
};

#endif
