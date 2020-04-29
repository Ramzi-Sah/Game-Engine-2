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

class PhysicsEntity {
private:

public:
    btRigidBody* rigidBody;
    Model* model;

    PhysicsEntity() {};
    virtual ~PhysicsEntity() {
        // delete collision box model
        delete model;

        // delete rigidBody from physics simulation
        Physics::deleteBody(rigidBody);
    };
};

class PhysicsEntity_Box : public PhysicsEntity {
private:

public:
    PhysicsEntity_Box(btVector3 size, float mass, btVector3 pos) {
        rigidBody = Physics::addBox(size, mass, pos);

        // handle collision box model
        model = new Model(ModelLoader::getModel("cube"));
        model->setScale(glm::vec3(size.getX(), size.getY(), size.getZ()));
        model->updateTransform();
        // model->setPolygoneMode(GL_LINE);

        model->meshGroups[0]->material.ambient = glm::vec3(0.0f, 0.75f, 1.0f);
        model->meshGroups[0]->material.diffuse = glm::vec3(0.0f, 0.75f, 1.0f);
        model->meshGroups[0]->material.specular = glm::vec3(0.0f, 0.0f, 0.0f);
        model->meshGroups[0]->material.opacity = 0.5f;
    };
};
class PhysicsEntity_Sphere : public PhysicsEntity {
private:

public:
    PhysicsEntity_Sphere(float radius, float mass, btVector3 pos) {
        rigidBody = Physics::addSphere(radius, mass, pos);

        // handle collision box model
        model = new Model(ModelLoader::getModel("sphere"));
        model->setScale(glm::vec3(radius, radius, radius));
        model->updateTransform();
        // model->setPolygoneMode(GL_LINE);

        model->meshGroups[0]->material.ambient = glm::vec3(0.0f, 0.75f, 1.0f);
        model->meshGroups[0]->material.diffuse = glm::vec3(0.0f, 0.75f, 1.0f);
        model->meshGroups[0]->material.specular = glm::vec3(0.0f, 0.0f, 0.0f);
        model->meshGroups[0]->material.opacity = 0.5f;
    };
};

#endif
