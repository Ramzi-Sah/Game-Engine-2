#ifndef SAH_ENTITY
#define SAH_ENTITY

#include <vector>

#include "../engine/physics/PhysicsEntity.hpp"
#include "../engine/Model.hpp"

class Entity {
private:
    btTransform transform;

    // physics
    bool isStatic = false;
    void setStatic(bool state);

    void setKinematic(bool state);
    bool isKinematic = false;
    bool kinematicTemp = false;

public:
    Entity(Model* _model, PhysicsEntity* _pysicsEntity);
    ~Entity();

    Model* model;
    PhysicsEntity* pysicsEntity;

    void update(float deltaTime);
    bool enabled = true;
    bool optimize = true;
    bool cameraAttached = false;

    // transform
    void setPos(glm::vec3 _position);
    void setKinematicPos(glm::vec3 _position);
    glm::vec3 getPos();
    void setRot(glm::quat _rotation);
    void setKinematicRot(glm::quat _rotation);
    glm::quat getRot();

    // physics
    void setStaticFlag(bool state);
    bool getIsStatic();

    void setKinematicFlag(bool state);
    void setKinematicTemp(bool state);
    bool getIsKinematic();

    // void setMass(float _mass);
    // float getMass();
    // float mass;

    bool renderCollider = false;

    // parameters
    glm::vec3 cameraOffsetPos = glm::vec3(0.0f, 0.0f, 0.0f);
    float cameraDistance = 30.0f;
};

#endif
