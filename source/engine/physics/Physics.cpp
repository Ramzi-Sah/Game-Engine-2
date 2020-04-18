#include "Physics.hpp"

btCollisionConfiguration* Physics::collisionConfig;
btDispatcher* Physics::dispatcher;
btBroadphaseInterface* Physics::broadPhase;
btConstraintSolver* Physics::solver;
btDynamicsWorld* Physics::world;
void Physics::init() {
    // create world
    collisionConfig = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfig);
	broadPhase = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver();
	world = new btDiscreteDynamicsWorld(dispatcher, broadPhase, solver, collisionConfig);

    // set world gravity
	world->setGravity(btVector3(0.0f, -9.98f, 0.0f));

    // handle debug draw
    world->setDebugDrawer(new PhysicsDebugDrawer());
    world->getDebugDrawer()->setDebugMode(btIDebugDraw::DBG_DrawWireframe); // btIDebugDraw::DBG_DrawWireframe btIDebugDraw::DBG_DrawAabb btIDebugDraw::DBG_DrawConstraints btIDebugDraw::DBG_DrawConstraintLimits
};

void Physics::update(float deltaTime) {
    // update similation
    world->stepSimulation(deltaTime);
};

void Physics::debugDraw() {
    // delete previous debug gui
    PhysicsDebugDrawer::dispose();

    // debug draw
    world->debugDrawWorld();
};

void Physics::dispose() {
    PhysicsDebugDrawer::dispose();

    delete world;
    delete solver;
    delete broadPhase;
    delete dispatcher;
    delete collisionConfig;
};

//--------------------------------------------------------------------------------
void Physics::addRigidBody(btRigidBody* body) {
    world->addRigidBody(body);
};
void Physics::deleteBody(btRigidBody* body) {
    // remove body from simulation
    world->removeCollisionObject(body);

    // delete instances
    delete body->getMotionState();
    delete body->getCollisionShape();
    delete body;
};

//--------------------------------------------------------------------------------
btRigidBody* Physics::addSphere(float radius, float mass, btVector3 pos) {
	// transforms
	btTransform t;
	t.setIdentity();
	t.setOrigin(pos);

    // create Sphere shape
	btSphereShape* sphere = new btSphereShape(radius);
	btMotionState* motion = new btDefaultMotionState(t);

    // create Sphere body
	btRigidBody::btRigidBodyConstructionInfo info = btRigidBody::btRigidBodyConstructionInfo(mass, motion, sphere, btVector3(1000.0f, 1000.0f, 1000.0f));
	btRigidBody* body = new btRigidBody(info);

    // set some attribs
    body->setActivationState(DISABLE_DEACTIVATION);
    body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT);

    body->setUserPointer(body);

	// add body to simulation
    world->addRigidBody(body);

	return body;
};
btRigidBody* Physics::addBox(btVector3 size, float mass, btVector3 pos) {
	// transforms
	btTransform t;
	t.setIdentity();
	t.setOrigin(pos);

    // create box shape
	btBoxShape* cube = new btBoxShape(size);
	btMotionState* motion = new btDefaultMotionState(t);

    // create box body
	btRigidBody::btRigidBodyConstructionInfo info = btRigidBody::btRigidBodyConstructionInfo(mass, motion, cube, btVector3(1000.0f, 1000.0f, 1000.0f));
    // info.m_localInertia = btVector3(1000.0f, 1000.0f, 1000.0f);
    // info.m_friction = 1.0f;

    // create rigidBody
	btRigidBody* body = new btRigidBody(info);

    // set some attribs
    body->setActivationState(DISABLE_DEACTIVATION);
    body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT);

    body->setUserPointer(body);

	// add body to simulation
    world->addRigidBody(body);

	return body;
};

//------------------------------------------------------------------------------
btRigidBody* Physics::rayCollisionCheck(unsigned int screenX, unsigned int screenY){
    // screen pos to ray
    Ray ray = Camera::rayCast(screenX, screenY);
    ray.setSize(10000);
    // std::cout << "rayPos: " << ray.position.x << " " << ray.position.y << " " << ray.position.z << " | rayDir: "<< ray.direction.x << " " << ray.direction.y << " " << ray.direction.z << '\n';

    // physics rayCallBack
    btCollisionWorld::ClosestRayResultCallback rayCallBack = btCollisionWorld::ClosestRayResultCallback(
        btVector3(
            ray.position.x,
            ray.position.y,
            ray.position.z
        ),
        btVector3(
            ray.direction.x,
            ray.direction.y,
            ray.direction.z
        )
    );
    world->rayTest(
        btVector3(
            ray.position.x,
            ray.position.y,
            ray.position.z
        ),
        btVector3(
            ray.direction.x,
            ray.direction.y,
            ray.direction.z
        ),
        rayCallBack
    );

    if (rayCallBack.hasHit()) {
        return (btRigidBody*)(rayCallBack.m_collisionObject->getUserPointer());
    };

    return nullptr;
};
