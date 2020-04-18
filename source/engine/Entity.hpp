#ifndef SAH_ENTITY
#define SAH_ENTITY

#include <vector>

#include "../engine/physics/Physics.hpp"
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

    void update();
    bool enabled = true;
    bool optimize = true;

    // transform
    void setPos(glm::vec3 _position);
    glm::vec3 getPos();
    void setRot(glm::quat _rotation);
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
};

#endif
