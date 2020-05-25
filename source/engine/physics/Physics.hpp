#ifndef SAH_PHYSICS
#define SAH_PHYSICS

#include <glm/glm.hpp>
#include <btBulletDynamicsCommon.h>

#include "DebugDrawer.hpp"
#include "../Model.hpp"
#include "../../common/Ray.hpp"

class Physics {
private:
    static btCollisionConfiguration* collisionConfig;
    static btDispatcher* dispatcher;
    static btBroadphaseInterface* broadPhase;
    static btConstraintSolver* solver;
    static btDynamicsWorld* world;

public:
    static void init();
    static void update(float deltaTime);
    static void debugDraw();
    static void dispose();

    static void addRigidBody(btRigidBody* body);
    static void deleteBody(btRigidBody* body);

    static btRigidBody* addSphere(float radius, float mass, btVector3 pos);
    static btRigidBody* addBox(btVector3 size, float mass, btVector3 pos);

    static btRigidBody* rayCollisionCheck(Ray ray);
};

#endif
